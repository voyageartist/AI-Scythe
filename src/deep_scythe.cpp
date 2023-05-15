/**
    deep_scythe.cpp
    Scythe's deep learning C API

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "deep_scythe.hpp"


CppClassesInterface cpp_classes_interface = CppClassesInterface();

scythe::DeepForest* CppClassesInterface::get(size_t i) {
   