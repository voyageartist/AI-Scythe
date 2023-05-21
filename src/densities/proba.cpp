/**
    proba.cpp
    Compute standard probability densities

    @author Antoine Passemiers
    @version 1.0 25/06/2017
*/

#include "proba.hpp"


namespace scythe {

Density* getArbitraryProbaDensities(size_t n_features, size_t n_classes) {
    assert(n_features > 0);
    Density* densities = new Density[n_features];
    densities[0].counters_left = new size_t[n_classes];
    densities[0].counters_right = new 