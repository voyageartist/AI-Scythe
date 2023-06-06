/**
    scanner1D.hpp
    Multi-grained scanning

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#ifndef SCANNER1D_HPP_
#define SCANNER1D_HPP_

#include "../../misc/sets.hpp"
#include "layer.hpp"

namespace scythe {

class ScannedDataset1D : public VirtualDataset {
private:
    size_t N;  // Number of instances
    size_t 