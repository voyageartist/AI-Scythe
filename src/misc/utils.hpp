/**
    utils.hpp
    Global use parameters
    
    @author Antoine Passemiers
    @version 1.0 24/06/2017
*/

#include <cassert>
#include <cstdint>

#include "numpy/npy_common.h"
#include "numpy/noprefix.h"

#include "exceptions.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP


namespace scythe {

typedef uint8_t BYTE;

constexpr int NPY_BOOL_NUM     =  0;
constexpr int NPY_INT8_NUM     =  1;
co