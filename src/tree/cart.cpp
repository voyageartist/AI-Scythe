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
        t