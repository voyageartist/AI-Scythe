
/**
    sets.cpp
    Virtual datasets
    
    @author Antoine Passemiers
    @version 1.0 09/06/2017
*/

#ifndef SETS_HPP_
#define SETS_HPP_

#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
#include <string>

#include "exceptions.hpp"
#include "utils.hpp"


namespace scythe {

constexpr size_t MAX_N_DIMS = 7;

constexpr int DTYPE_PROBA   = 0;
constexpr int DTYPE_DOUBLE  = 1;
constexpr int DTYPE_UINT_8  = 2;


typedef unsigned int uint;
typedef double       data_t;
typedef float        fast_data_t;
typedef double       target_t;
typedef float        proba_t;
typedef int          label_t;

// Forward declarations
class VirtualDataset;
class VirtualTargets;
/*
C++ note: Neither alias declarations or forward declarations work with
nested classes since the compiler can't be sure that a nested class
actually exists.
*/

typedef std::shared_ptr<VirtualDataset> vdataset_p;
typedef std::shared_ptr<VirtualTargets> vtargets_p;

// Data samples without target values
struct Dataset {