/**
    classification_forest.cpp
    Classification random forests

    @author Antoine Passemiers
    @version 1.0 07/06/2017
*/

#include "classification_rf.hpp"


namespace scythe {

ClassificationRF::ClassificationRF
        (ForestConfig* config, size_t n_instances, size_t n_features) :
        ClassificationForest::ClassificationForest(config, n_instances, n_features) {
    Forest::base_tree_config.task = CLASSIFICATION_TASK;
    Forest::base_tree_config.is_complete_random = (config->type == COMPLETE_RANDOM_FOREST); 