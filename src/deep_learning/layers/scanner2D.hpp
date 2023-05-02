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
    size_t kc; // Kernel width
    size_t kr; // Kernel height
    size_t sc; // Number of kernel positions per column
    size_t sr; // Number of kernel positions per row

    size_t Nprime; // Number of instances after scanning
    size_t Mprime; // Number of features after scanning

    void* data; // Pointer to the raw data
    int dtype;    // Raw data type

    // Iterator cursors
    size_t _it_x;
    size_t _it_i;
    size_t _it_q;
public:
    ScannedDataset2D(void* data, size_t N, size_t M, 
        size_t P, size_t kc, size_t kr, int dtype);
    ScannedDataset2D(const ScannedDataset2D& other) = default;
    ScannedDataset2D& operator=(const ScannedDataset2D& other) = default;
    ~ScannedDataset2D() override = default;
    virtual VirtualDataset* deepcopy();
    virtual VirtualDataset* createView(void* view, size_t n_rows);
    virtual data_t operator()(size_t i, size_t j);

    // Virtual iterator
    virtual void _iterator_begin(const size_t j);
    virtual void _iterator_inc();
    virtual data_t _iterator_deref();
    inline void inline_iterator_begin(const size_t j);
    inline void inline_iterator_inc();
    inline data_t inline_iterator_deref();

    template<typename T, typename fast_T>
    void generic_allocateFromSampleMask(size_t* const mask, size_t, size_t, size_t, size_t);
    virtual void allocateFromSampleMask(size_t* const mask, size_t, size_t, size_t, size_t);

    // Getters
    size_t getSc() { return sc; }
    size_t getSr() { return sr; }
    virtual size_t getNumInstances() { return Nprime; }
    virtual size_t getNumFeatures() { return Mprime; }
    virtual size_t getNumVirtualInstancesPerInstance() { return sc * sr; }
    virtual size_t getNumRows() { return N; }
    virtual size_t getRowStride() { return M * P; }
    virtual int    getDataType() { return dtype; }
    virtual void*  getData() { return data; }
};


class ScannedTargets2D : public VirtualTargets {
private:
    target_t* data;
    size_t n_rows;
    size_t sc;
    size_t sr;
    size_t s;
    size_t _it_x;
    size_t _it_i;
public:
    ScannedTargets2D(target_t* data, size_t n_instances, size_t sc, size_t sr);
  