/**
    pruning.hpp
    Post-pruning on classification and regression trees

    @author Antoine Passemiers
    @version 1.0 09/08/2017
*/

#include "pruning.hpp"


namespace scythe {

Scythe::Scythe() : trees(), prunings(), n_prunings(0), delete_branches(false) {}

size_t Scythe::cut(Node* root) {
    size_t n_removed_nodes = 0;
    std::queue<Node*> queue;
    queue.push(root->left_child);
    queue.push(root->right_child);
    while (!queue.empty()) {
        Node* node = queue.fro