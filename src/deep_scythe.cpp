/**
    deep_scythe.cpp
    Scythe's deep learning C API

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "deep_scythe.hpp"


CppClassesInterface cpp_classes_interface = CppClassesInterface();

scythe::DeepForest* CppClassesInterface::get(size_t i) {
    return df_ptrs.at(i);
}

extern "C" {

    size_t c_create_deep_forest(int task) {
        scythe::DeepForest* forest = new scythe::DeepForest(task);
        size_t ptr_id = cpp_classes_interface.num_df_ptrs++;
        cpp_classes_interface.df_ptrs.push_back(forest);
        return ptr_id;
    }

    void c_fit_deep_forest(
        scythe::MDDataset dataset, scythe::Labels* labels, size_t forest_id) {
        scythe::DeepForest* forest = cpp_classes_interface.get(forest_id);
        forest->fit(dataset, labels);
    }

    float* c_deep_forest_classify(scythe::MDDataset dataset, size_t forest_id) {
        scythe::DeepForest* forest = cpp_classes_interface.get(forest_id);
        return forest->classify(da