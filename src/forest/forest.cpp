/**
    forest.cpp
    Forest abstract class and configurations

    @author Antoine Passemiers
    @version 1.0 23/06/2017
*/

#include "forest.hpp"


namespace scythe {

void Forest::preprocessDensities(VirtualDataset* dataset) {
    if (dataset->getDataType() == DTYPE_UINT_8) {
        /**
        this