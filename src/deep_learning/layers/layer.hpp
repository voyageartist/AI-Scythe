
/**
    layer.hpp
    Deep learning base layer

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#ifndef LAYER_HPP_
#define LAYER_HPP_

#include "../../forest/forest.hpp"
#include "../../forest/classification_rf.hpp"
#include "../../forest/classification_gb.hpp"
// #include "../../forest/regression_rf.hpp"
// #include "../../forest/regression_gb.hpp"

namespace scythe {

// Forward declarations
class Layer;
class ConcatenationDataset;

typedef std::shared_ptr<Layer> layer_p;

struct LayerConfig {
    ForestConfig fconfig;
    size_t       n_forests;
    int          forest_type;
    LayerConfig();
};

/**
    Main goal of layers: ensuring that each forest gets
    a two-dimensional dataset as input, and ensuring that
    the dimensionality of the output is right
    (1d for regression, 2d for classification). This dimensionality
    must be invariant to the complexity of cascades and convolutional layers.

    Therefore, the shapes of the datasets are "re-mapped" between layers, and
    the definition of how it works must be defined in each layer class.
*/

class Layer {
protected:
    std::string name; // Layer name

    // The product of in_shape elements must be equal to the product of
    // virtual_in_shape elements.
    std::vector<size_t> in_shape;          // Input shape
    std::vector<size_t> virtual_in_shape;  // Virtual input shape
    std::vector<size_t> virtual_out_shape; // Virtual output shape

    std::vector<layer_p> children; // child layers
    std::vector<layer_p> parents;  // parent layers
    std::vector<std::shared_ptr<Forest>> forests; // Intern forests

    vdataset_p vdataset; // Virtual dataset
    vtargets_p vtargets; // Virtual target values
    LayerConfig lconfig; // Layer configuration

    bool grown = false; // Indicates whether the layer has learned or not

public:
    Layer(LayerConfig lconfig);
    virtual ~Layer() = 0;
    void addChild(layer_p layer);
    void addParent(layer_p layer);
    virtual vdataset_p virtualize(MDDataset dataset) = 0;
    virtual vtargets_p virtualizeTargets(Labels* targets) = 0;
    size_t getNumChildren();
    size_t getNumParents();
    vdataset_p getVirtualDataset();
    virtual size_t getNumVirtualFeatures() = 0;
    virtual std::string getType() = 0;
    virtual bool isConcatenable() = 0;
    void setTask(int task) { lconfig.fconfig.task = task; }
    void grow(vdataset_p vdataset, vtargets_p vtargets);
    float* classify(vdataset_p vdataset);

    std::vector<layer_p> getChildren() { return children; }
    std::vector<layer_p> getParents() { return parents; }
    std::vector<std::shared_ptr<Forest>> getForests() { return forests; }
    size_t getNumForests() { return getForests().size(); }
    size_t getNumClasses() { return lconfig.fconfig.n_classes; }
    std::shared_ptr<Forest> getForestByID(size_t forest_id) { return forests.at(forest_id); }
    bool isClassifier() { return (lconfig.fconfig.task == CLASSIFICATION_TASK); }
};

std::ostream& operator<<(std::ostream& os, Layer* const layer);

} // namespace

#endif // LAYER_HPP_