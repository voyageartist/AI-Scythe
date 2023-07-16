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
    root(root), n_no