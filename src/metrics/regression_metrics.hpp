/**
    regression_metrics.hpp
    Metrics for evaluating regression forests

    @author Antoine Passemiers
    @version 1.0 12/04/2017
*/

#ifndef REGRESSION_METRICS_HPP_
#define REGRESSION_METRICS_HPP_

#include "metrics.hpp"


namespace scythe {

class RegressionError {
protected:
    size_t n_classes;
    size_t n_instances;
    std::shared_ptr<data_t> gradient;
public:
    inline size_t getNumberOfClasses() { return this->n_classes; }
    inline size_t getNumberOfInstances() { return this->n_instances; }
  