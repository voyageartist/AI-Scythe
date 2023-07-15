
/**
    sets.cpp
    Datasets' structures
    
    @author Antoine Passemiers
    @version 1.0 09/06/2017
*/

#include "sets.hpp"


namespace scythe {

size_t VirtualDataset::getItemStride() {
    switch (getDataType()) {
        case NPY_BOOL_NUM:
            return sizeof(npy_bool);
        case NPY_INT8_NUM:
            return sizeof(npy_int8);
        case NPY_UINT8_NUM:
            return sizeof(npy_uint8);
        case NPY_INT16_NUM:
            return sizeof(npy_int16);
        case NPY_UINT16_NUM:
            return sizeof(npy_uint16);
        case NPY_INT32_NUM:
            return sizeof(npy_int32);
        case NPY_UINT32_NUM:
            return sizeof(npy_uint32);
        case NPY_INT64_NUM:
            return sizeof(npy_int64);
        case NPY_UINT64_NUM:
            return sizeof(npy_uint64);
        case NPY_FLOAT32_NUM:
            return sizeof(npy_float32);
        case NPY_FLOAT64_NUM:
            return sizeof(npy_float64);
        case NPY_FLOAT16_NUM:
            return sizeof(npy_float16);
        default:
            throw UnhandledDtypeException();
    }
}
