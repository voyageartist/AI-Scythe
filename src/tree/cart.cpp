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
    std::queue<Node*> 