/**
    concatenation_layer.cpp
    Base layer with concatenation of two feature matrices

    @author Antoine Passemiers
    @version 1.0 23/06/2017
*/

#include "concatenation_layer.hpp"


namespace scythe {

ConcatenationDataset::ConcatenationDataset(
    proba_t* data, size_t n_instances, size_t n_virtual_cols, size_t stride, int dtype) :
    data(data), n_instances(n_instances), n_virtual_cols(n_virtual_cols), stride(stride), dtype(dtype) {}