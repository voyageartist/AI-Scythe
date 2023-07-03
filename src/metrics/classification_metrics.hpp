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

http://luthuli.cs.uiuc.edu/~daf/courses