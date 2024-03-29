/**
    pruning.hpp
    Post-pruning on classification and regression trees

    @author Antoine Passemiers
    @version 1.0 09/08/2017
*/

#include "pruning.hpp"


namespace scythe {

Scythe::Scythe() : trees(), prunings(), n_prunings(0), delete_branches(false) 