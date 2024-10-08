
/**
    classification_gb.cpp
    Classification gradient boosting
    @author Antoine Passemiers
    @version 1.0 12/04/2017
*/

#ifndef CLASSIFICATION_GB_HPP_
#define CLASSIFICATION_GB_HPP_

#include "forest.hpp"
#include "../metrics/classification_metrics.hpp"


namespace scythe {

class ClassificationGB : public Forest {
private:
    std::shared_ptr<ClassificationError> score_metric;
    TreeConfig grad_trees_config;
public:
    ClassificationGB(ForestConfig*, size_t, size_t);
    void fit(VirtualDataset* dataset, VirtualTargets* targets);
    float* fitBaseTree(VirtualDataset* dataset, VirtualTargets* targets);
    void fitNewTree(VirtualDataset* dataset, VirtualTargets* gradient);
    data_t* predictGradient(std::shared_ptr<Tree> tree, VirtualDataset* dataset);
    void applySoftmax(float* probabilities, data_t* F_k);
    ~ClassificationGB() = default;
};

}

#endif // CLASSIFICATION_GB_HPP_