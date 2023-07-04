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
    inline size_t g