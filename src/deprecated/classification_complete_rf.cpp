/**
    classification_complete_rf.cpp
    Classification completely-random forests

    @author Antoine Passemiers
    @version 1.0 13/06/2017
*/

#include "classification_complete_rf.hpp"


namespace scythe {

ClassificationCompleteRF::ClassificationCompleteRF
        (ForestConfig* config, size_t n_instances, size_t n_features) :
        ClassificationForest::ClassificationForest(config, n_instances, n_features) {
    Forest::base_tree_config.task = CLASSIFICATION_TASK;
    Forest::base_tree_config.is