/**
    heuristics.cpp
    Meta-heuristics for increasing CART's speed

    @author Antoine Passemiers
    @version 1.3 10/06/2017
*/

#include "heuristics.hpp"


namespace scythe {

SplitManager::SplitManager(Density* const densities, size_t n_features) : 
    n_features(n_features), n_grown_trees(0), features() {

    feature_importances = new double[n_features]();
    for (size_t f = 0; f < n_features; f++) {
        std::shared_ptr<FeatureInfo> feature(
            static_cast<FeatureInfo*>(malloc(sizeof(FeatureInfo))));
        size_t n_values = densities[f].n_values;
        feature->ntimes_best = new size_t[n_values]();
        feature->n_values = n_values;
  