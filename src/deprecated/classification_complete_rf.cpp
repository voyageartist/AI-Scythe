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
    Forest::base_tree_config.is_complete_random = true;
    if ((Forest::base_tree_config.max_n_features > n_features) ||
        (Forest::base_tree_config.max_n_features == 0)) {
        Forest::base_tree_config.max_n_features = n_features;
    }
    /*
    this->score_metric = std::move(
        std::shared_ptr<ClassificationError>(
            new MultiLogLossError(config->n_classes, n_instances)));
    */
}

void ClassificationCompleteRF::fitNewTree(VirtualDataset* dataset, VirtualTargets* targets) {
    assert(dataset->getNumInstances() == targets->getNumInstances());
    size_t n_rows = targets->getNumInstances();
    size_t bag_size = config.bagging_fraction * n_rows;
    std::vector<size_t> indexes = randomSet(bag_size, n_rows);
    VirtualDataset* dataset_view = dataset->shuffleAndCreateView(indexes);
    VirtualTargets* targets_view = targets->shuffleAndCreateView(indexes);

    std::shared_ptr<Tree> new_tree = std::shared_ptr<Tree>(CART(
        dataset_view,
        targets_view, 
        &(Forest::base_tree_confi