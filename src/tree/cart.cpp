/**
    cart.cpp
    Grow classification trees and regression trees

    @author Antoine Passemiers
    @version 1.4 27/11/2017
*/

#include "cart.hpp"


namespace scythe {

Tree::Tree() :
    root(nullptr), n_nodes(0), n_classes(0), 
    n_features(0), config(nullptr), level(1), split_manager(nullptr) {}

Tree::Tree(Node* root, TreeConfig* config, size_t n_features) :
    root(root), n_nodes(1), n_classes(config->n_classes),
    n_features(n_features), config(config), level(1), split_manager(nullptr) {}

Tree::Tree(const Tree& other) {
    Node* nodes = new Node[other.n_nodes];
    this->root = &nodes[0];
    this->n_nodes = other.n_nodes;
    this->n_classes = other.n_classes;
    this->n_features = other.n_features;
    this->config = other.config;
    this->level = other.level;
    this->split_manager = other.split_manager;

    size_t current_node_id = 0;
    std::queue<Node*> queue;
    queue.push(other.root);
    while (!queue.empty()) {
        Node* next_node = queue.front(); queue.pop();
        std::memcpy(&nodes[current_node_id], next_node, sizeof(Node));
        current_node_id++;
        if (next_node->left_child != nullptr) {
            queue.push(next_node->left_child);
            queue.push(next_node->right_child);
        }
    }
}

Node::Node(size_t n_classes, int id, size_t n_instances):
    id(id),
    n_instances(n_instances),
    counters(n_classes > 0 ? new (std::nothrow) size_t[n_classes] : nullptr) {

    memset(counters, 0x00, n_classes * sizeof(size_t));
    }

NodeSpace::NodeSpace(Node* owner, size_t n_features, Density* densities) :
    owner(owner),
    current_depth(1),
    feature_left_bounds(new size_t[n_features]),
    feature_right_bounds(new size_t[n_features]) {
    /**
        This constructor is called while instantiating the tree's root. 
        When evaluating the split values for
        the root, all the partition values of each features are being considered,
        this is why the left and right bounds of the node space are first set 
        to their maxima.

        @param owner
            Node related to the current space (root of the tree)
        @param n_features
            Number of features in the training set
        @param densities
            Density functions of the features
            There are supposed to be n_features densities
        @return A new NodeSpace for the tree's root
    */
    for (uint f = 0; f < n_features; f++) {
        feature_left_bounds[f] = 1;
        feature_right_bounds[f] = densities[f].n_values;
    }
}

NodeSpace::NodeSpace(const NodeSpace& node_space, size_t n_features) :
    owner(node_space.owner), 
    current_depth(node_space.current_depth),
    feature_left_bounds(new size_t[n_features]),
    feature_right_bounds(new size_t[n_features]) {
    size_t n_bytes = n_features * sizeof(size_t);
    memcpy(feature_left_bounds, node_space.feature_left_bounds, n_bytes);
    memcpy(feature_right_bounds, node_space.feature_right_bounds, n_bytes);
}

Splitter::Splitter(NodeSpace node_space, TreeConfig* config, size_t n_instances,
        size_t n_features, size_t* belongs_to, VirtualTargets* targets, SplitManager* split_manager) :
    task(config->task), 
    node(node_space.owner),
    n_instances(n_instances),
    n_instances_in_node(n_instances),
    n_classes(config->n_classes),
    mean_left(0.0),
    mean_right(0.0),
    n_left(0),
    n_right(0),
    belongs_to(belongs_to),
    feature_id(static_cast<size_t>(NO_FEATURE)),
    n_features(n_features),
    targets(targets),
    nan_value(config->nan_value),
    best_split_id(0),
    node_space(node_space),
    is_complete_random(config->is_complete_random),
    split_manager(split_manager),
    class_weights(config->class_weights) {}


void ordered_push(std::list<NodeSpace>& queue, NodeSpace nodespace, bool ordered) {
    if (ordered) {
        bool inserted = false;
        for (std::list<NodeSpace>::iterator it = queue.begin(); it != queue.end(); ++it) {
            if ((*it).information_gain <= nodespace.information_gain) {
                queue.insert(it, nodespace);
                inserted = true;
                break;
            }
        }
        if (!inserted) queue.push_back(nodespace);
    }
    else {
        queue.push_back(nodespace);
    }
}

double getFeatureCost(size_t* RESTRICT const counters_left, 
    size_t* RESTRICT const counters_right, size_t n_classes, float* class_weights) {

    size_t n_left = sum_counts(counters_left, n_classes);
    size_t n_right = sum_counts(counters_right, n_classes);
    if (n_left == 0 || n_right == 0) {
        return COST_OF_EMPTINESS;
    }
    double left_cost = 0.0;
    for (uint i = 0; i < n_classes; i++) {
        left_cost += pow2(static_cast<float>(counters_left[i]) / static_cast<float>(n_left));
    }
    left_cost = (1.0 - left_cost);
    double right_cost = 0.0;
    for (uint i = 0; i < n_classes; i++) {
        right_cost += pow2(static_cast<float>(counters_right[i]) / static_cast<float>(n_right));
    }
    right_cost = (1.0 - right_cost);
    float left_rate = static_cast<float>(n_left) / static_cast<float>(n_left + n_right);
    return left_cost * left_rate + right_cost * (1.0 - left_rate);
}

double informationGain(
    size_t* counters, size_t* counters_left, size_t* counters_right, size_t n_classes, float* class_weights) {

    double gini = getFeatureCost(counters_left, counters_right, n_classes, class_weights);
    size_t n_total = sum_counts(counters, n_classes);
    double cost = 0.0;
    for (uint i = 0; i < n_classes; i++) {
        cost += pow2(static_cast<float>(counters[i]) / static_cast<float>(n_total));
    }
    cost = (1.0 - cost);
    double gain = cost - gini;
    gain = (gain < 0.0) ? 0.0 : gain;
    return gain;
}

double evaluatePartitions(VirtualDataset* RESTRICT data, const Density* RESTRICT density, 
    const Splitter* RESTRICT splitter, double split_value_double) {
    size_t* counters_left = density->counters_left;
    size_t* counters_right = density->counters_right;
    std::fill(counters_left, counters_left + splitter->n_classes, 0);
    std::fill(counters_right, counters_right + splitter->n_classes, 0);
    fast_data_t split_value = static_cast<fast_data_t>(split_value_double);
    
    label_t* RESTRICT contiguous_labels = (*(splitter->targets)).retrieveContiguousData();

    size_t* counter_ptrs[2] = { counters_left, counters_right };

    switch (data->getDataType()) {
        case NPY_UINT8_NUM:
            count_instances(static_cast<uint8_t*>(data->retrieveContiguousData()),
                contiguous_labels, counter_ptrs, splitter->n_instances_in_node, split_value, splitter->nan_value);
            break;
        default:
            count_instances(static_cast<fast_data_t*>(data->retrieveContiguousData()), 
                contiguous_labels, counter_ptrs, splitter->n_instances_in_node, split_value, splitter->nan_value);
            break;
    }
    return getFeatureCost(counters_left, counters_right, splitter->n_classes, splitter->class_weights);
}

double evaluatePartitionsWithRegression(VirtualDataset* data, Density* density, Splitter* splitter, size_t k) {
    size_t i = splitter->feature_id;
    data_t data_point, nan_value = splitter->nan_value;
    double y;
    size_t id = splitter->node->id;
    size_t* belongs_to = splitter->belongs_to;
    size_t n_left = 0, n_right = 0;
    density->split_value = density->values[k];
    VirtualTargets* targets = splitter->targets;
    data_t split_value = density->split_value;
    double mean_left = 0.0, mean_right = 0.0;
    double cost = 0.0;

    for (uint j = 0; j < splitter->n_instances; j++) {
        if (belongs_to[j] == id) {
            data_point = (*data)(j, i);
            y = static_cast<double>((*targets)[j]);
            // if (data_point == nan_value) {}
            if (data_point >= split_value) {
                mean_right += y;
                n_right++;
            }
            else {
                mean_left += y;
                n_left++;
            }
        }
    }
    mean_left /= static_cast<double>(n_left);
    mean_right /= static_cast<double>(n_right);
    splitter->mean_left  = mean_left;
    splitter->mean_right = mean_right;
    splitter->n_left = n_left;
    splitter->n_right = n_right;
    if ((n_left == 0) || (n_right == 0)) { return INFINITY; }
    for (uint j = 0; j < splitter->n_instances; j++) {
        if (splitter->belongs_to[j] == id) {
            data_point = (*data)(j, i);
            y = (*targets)[j];
            // if (data_point == splitter->nan_value) {}
            if (data_point >= split_value) {
                cost += abs(mean_right - y); // TODO : use squared error ?
            }
            else {
                cost += abs(mean_left - y); // TODO : use squared error ?
            }
        }
    }
    // printf("Cost : %f", cost);
    return cost;
}

double evaluateBySingleThreshold(Splitter* splitter, Density* density, const VirtualDataset* data) {
    return 0.0; // TODO
}

double evaluateByThreshold(Splitter* splitter, Density* density, VirtualDataset* data) {
    size_t lower_bound = splitter->node_space.feature_left_bounds[splitter->feature_id];
    size_t upper_bound = splitter->node_space.feature_right_bounds[splitter->feature_id];
    if (lower_bound == upper_bound) { return INFINITY; }
    if (splitter->is_complete_random) {
        // return evaluateBySingleThreshold(splitter, density, data);
        size_t random_bound = lower_bound + (rand() % (upper_bound - lower_bound));
        lower_bound = random_bound;
        upper_bound = random_bound + 1;
    }
    size_t n_classes = splitter->n_classes;

    data->allocateFromSampleMask(
        splitter->belongs_to,
        splitter->node->id,
        splitter->feature_id,
        splitter->n_instances_in_node,
        splitter->n_instances);
    splitter->targets->allocateFromSampleMask(
        splitter->belongs_to,
        splitter->node->id,
        splitter->n_instances_in_node,
        splitter->n_instances);

    size_t best_split_id = 0;
    double lowest_cost = INFINITY;
    size_t best_counters_left[MAX_N_CLASSES];
    size_t best_counters_right[MAX_N_CLASSES];
    for (size_t k = lower_bound; k < upper_bound; k++) {
        if (splitter->split_manager->shouldEvaluate(splitter->feature_id, k)) {
            double cost;
            if (splitter->task == CLASSIFICATION_TASK) {
                density->split_value = density->values[k];
                cost = evaluatePartiti