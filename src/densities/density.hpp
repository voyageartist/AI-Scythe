/**
    density.hpp
    Basic density structure of a set of features

    @author Antoine Passemiers
    @version 1.3 12/04/2017
*/

#ifndef DENSITY_HPP_
#define DENSITY_HPP_

#include <numeric>
// #include <algorithm.h>

#include "../misc/sets.hpp"


namespace scythe {

// Forward declaration
class SplitManager;

// Partitioning of the input's density function
constexpr int