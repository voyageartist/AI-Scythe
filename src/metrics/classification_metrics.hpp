/**
    classification_metrics.hpp
    Metrics for evaluating classification forests

    @author Antoine Passemiers
    @version 1.0 12/04/2017
*/

#ifndef CLASSIFICATION_METRICS_HPP_
#define CLASSIFICATION_METRICS_HPP_

#include "metrics.hpp"
#include "../tree/cart.hpp"
#include "../misc/sets.hpp"

/*
Reference
---------

http://luthuli.cs.uiuc.edu/~daf/courses/optimization/papers/2699986.pdf
pg 1201
*/

namespace scythe {

class ClassificationError {
protected:
    size_t n_classes;
    size_t n_instances;
    std::shared_ptr<target_t> gradient;
public:
    inline size_t getNumberOfClasses() { return this->n_classes; }
    inline size_t getNumberOfInstances() { return this->n_instances; }
    virtual size_t getNumberOfRequiredTrees() = 0;
    virtual void computeGradient(float* const, target_t* const) = 0;
    virtual loss_t computeLoss(float* const, VirtualTargets* const) = 0;
    ClassificationError();
    ClassificationError(size_t n_classes, size_t n_instances);
    virtual ~ClassificationError();
};

class MultiLogLossError : public ClassificationError {
private:
    double stability_threshold = 10.0e-15;
public:
    MultiLogLossError(size_t n_classes, size_t n_instances) {
        ClassificationError::n_classes = n_classes;
        ClassificationError::n_instances = n_instances;
        ClassificationError::gradient = std::move(std::shared_ptr<target_t>(new target_t[n_classes * n_instances]));
    }

    inline size_t getNumberOfRequiredTrees() { return this->n_classes; }

    inline double getStabilityThreshold() { return t