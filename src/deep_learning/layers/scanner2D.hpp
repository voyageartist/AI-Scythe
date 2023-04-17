/**
    scanner2D.hpp
    Multi-grained scanning

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#ifndef SCANNER2D_HPP_
#define SCANNER2D_HPP_

#include "../../misc/sets.hpp"
#include "layer.hpp"

namespace scythe {

class ScannedDataset2D : public VirtualDataset {
private:
    size_t N;  // Number of instances
    size_t M;  // Instance height
    size_t P;  // Instance width
    size_t 