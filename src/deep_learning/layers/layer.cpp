
/**
    layer.hpp
    Deep learning base layer

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "layer.hpp"


namespace scythe {

LayerConfig::LayerConfig() :
     fconfig(), n_forests(0), forest_type(COMPLETE_RANDOM_FOREST) {}

Layer::Layer(LayerConfig lconfig) :
    name(), in_shape(), virtual_in_shape(), virtual_out_shape(), 
    children(), parents(), forests(), vdataset(nullptr), vtargets(nullptr), 
    lconfig(lconfig) {}

Layer::~Layer() {}

void Layer::grow(vdataset_p vdataset, vtargets_p vtargets) {
    assert(!grown);
    assert(forests.size() == 0);
    Forest* forest;
    assert(vdataset->getNumInstances() == vtargets->getNumInstances());
    for (unsigned int i = 0; i < lconfig.n_forests; i++) {
        if ((lconfig.forest_type == RANDOM_FOREST) || (lconfig.forest_type == COMPLETE_RANDOM_FOREST)) {
            forest = new ClassificationRF(
                &lconfig.fconfig, 
                vdataset->getNumInstances(), 
                vdataset->getNumFeatures());
        }
        else if (lconfig.forest_type == GB_FOREST) {
            // forest = new ClassificationGB(config, dataset->n_rows, dataset->n_cols);
            std::cout << "Error: gradient boosting is not supported" << std::endl;
        }
        else {
            std::cout << "Error: this type of forest does not exist" << std::endl;
        }
        forest->fit(vdataset.get(), vtargets.get());
        forests.push_back(std::shared_ptr<Forest>(forest));
    }
    grown = true;
}

float* Layer::classify(vdataset_p vdataset) {
    assert(grown);
    size_t n_instances = vdataset->getNumInstances();
    float* predictions = static_cast<float*>(calloc(n_instances * getNumClasses(), sizeof(float)));
    ClassificationForest* forest_p;
    for (unsigned int i = 0; i < lconfig.n_forests; i++) {
        forest_p = dynamic_cast<ClassificationForest*>(forests.at(i).get());
        float* local_predictions = forest_p->classify(vdataset.get());
        for (unsigned int j = 0; j < n_instances * getNumClasses(); j++) {
            predictions[j] += local_predictions[j];
        }
        delete[] local_predictions;
    }
    for (unsigned int j = 0; j < n_instances * getNumClasses(); j++) {
        predictions[j] /= lconfig.n_forests;
    }
    return predictions;
}

void Layer::addChild(layer_p layer) {
    children.push_back(layer);
}

void Layer::addParent(layer_p layer) {
    parents.push_back(layer);
}

vdataset_p Layer::getVirtualDataset() {
    return this->vdataset;
}

size_t Layer::getNumChildren() {
    return this->children.size();
}

size_t Layer::getNumParents() {
    return this->parents.size();
}

std::ostream& operator<<(std::ostream& os, Layer* const layer) {
    return os << "" << std::endl; // TODO
}

} // namespace