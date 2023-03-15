/**
    deep_forest.hpp
    Deep forest

    @author Antoine Passemiers
    @version 1.0 12/06/2017
*/

#ifndef DEEP_FOREST_HPP_
#define DEEP_FOREST_HPP_

#include <algorithm>
#include <queue>
#include <iomanip>

#include "layers/layer.hpp"
#include "layers/concatenation_layer.hpp"


namespace scythe {

class DeepForest {
private:
    std::vector<layer