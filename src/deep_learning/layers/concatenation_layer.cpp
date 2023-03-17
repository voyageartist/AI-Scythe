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

ConcatenationDataset::ConcatenationDataset(size_t n_instances, size_t n_virtual_features) :
    data(new proba_t[n_instances * n_virtual_features]),
    n_instances(n_instances),
    n_virtual_cols(n_virtual_features),
    stride(0),
    dtype(NPY_FLOAT32_NUM) {} // TODO : dtype in case of regresion task

VirtualDataset* ConcatenationDataset::deepcopy() {
    size_t n_required_bytes = getNumRows() * getRowStride() * getItemStride();
    void* new_data = malloc(n_require