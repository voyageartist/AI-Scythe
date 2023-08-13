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
    return left_cost * left_rate + right_cost