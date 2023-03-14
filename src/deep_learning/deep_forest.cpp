
/**
    deep_forest.cpp
    Deep forest

    @author Antoine Passemiers
    @version 1.0 12/06/2017
*/

#include "deep_forest.hpp"


namespace scythe {

DeepForest::DeepForest(int task) : 
    layers(),
    n_layers(0),
    task(task), 
    front(nullptr), 
    rear(nullptr), 
    cascade_buffers() {}

size_t DeepForest::add(layer_p layer) {
    layer->setTask(task);
    layers.push_back(layer);
    if (front.get() == nullptr) {
        front = layer;
        assert(layer.get() != nullptr);
        assert(front.get() == layer.get());
    }
    if (rear.get() != nullptr) {
        rear->addChild(layer);
        layer->addParent(rear);
    }
    rear = layer;
    assert(rear.get() == layer.get());
    return n_layers++;
}

void DeepForest::connect(layer_p parent, layer_p child) {
    assert(parent.get() != child.get());
    rear = child;
    // TODO: assert child and parent in this.layers
    parent->addChild(child);
    child->addParent(parent);
}

void DeepForest::printGraph() {
    size_t layer_id = 1;
    std::cout << std::endl;
    for (layer_p layer : layers) {
        std::cout << "----------------------" << std::endl;
        std::cout << "Layer " << std::setfill(' ') << std::setw(3) << layer_id << std::endl;
        std::cout << "        | Type       : " << layer->getType() << std::endl;