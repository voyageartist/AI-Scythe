/**
    exceptions.hpp
    Custom exceptions
    
    @author Antoine Passemiers
    @version 1.0 24/06/2017
*/

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <stdexcept>


namespace scythe {

class OOPException : public std::runtime_error {
public:
    // When an overr