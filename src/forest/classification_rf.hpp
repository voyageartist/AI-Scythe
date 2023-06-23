/**
    classification_rf.cpp
    Classification random forests
    
    @author Antoine Passemiers
    @version 1.0 12/04/2017
*/

#ifndef CLASSIFICATION_RF_HPP_
#define CLASSIFICATION_RF_HPP_

#include "../misc/sets.hpp"
#include "forest.hpp"
#include "../metrics/classification_metrics.hpp"
#include "../misc/bagging.hpp"


namespace scythe {

class ClassificationRF : public ClassificationForest {
private:
    std::shared_ptr<ClassificationError> score_metric;
public:
    ClassificationRF(ForestConfig*, size_t, size_t);
    void init();
    void fit(VirtualDataset* dataset, Virtua