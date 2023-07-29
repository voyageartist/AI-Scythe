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

Splitter::Splitter(NodeSpace node_space, TreeConfi