/**
    concatenation_layer.hpp
    Base layer with concatenation of two feature matrices

    @author Antoine Passemiers
    @version 1.0 23/06/2017
*/

#ifndef CONCATENATION_LAYER_HPP_
#define CONCATENATION_LAYER_HPP_

#include "layer.hpp"


namespace scythe {

class ConcatenationDataset : public VirtualDataset {    
private:
  