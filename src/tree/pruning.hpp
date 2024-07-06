/**
    pruning.hpp
    Post-pruning on classification and regression trees

    @author Antoine Passemiers
    @version 1.0 09/08/2017
*/

#ifndef PRUNING_HPP_
#define PRUNING_HPP_

#include "../tree/cart.hpp"


namespace scythe {


struct NodeLevel {
    Node* owner;
    