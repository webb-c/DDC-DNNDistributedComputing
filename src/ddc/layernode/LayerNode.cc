/*
 * LayerNode.cc
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#include "ddc/layernode/LayerNode.h"

namespace inet {

    LayerNode::LayerNode() : LayerNode(-1, -1) {}

    LayerNode::LayerNode(int node_number, int layer_number){
        this->node_number = node_number;
        this->layer_number = layer_number;
    }


    int LayerNode::getNodeNumber() const {
        return this->node_number;
    }

    int LayerNode::getLayerNumber() const {
        return this->layer_number;
    }

    bool LayerNode::isSameLayer(LayerNode target_layer_node) const {
        if (this->layer_number == target_layer_node.getLayerNumber())
            return true;
        else
            return false;
    }

    bool LayerNode::isSameNode(LayerNode target_layer_node) const {
        if (this->node_number == target_layer_node.getNodeNumber())
            return true;
        else
            return false;
    }

    bool LayerNode::isSameLayerNode(LayerNode target_layer_node) const {
        if (this->isSameLayer(target_layer_node) && this->isSameNode(target_layer_node))
            return true;
        else
            return false;
    }

    string LayerNode::toString() const{
        return "node" + to_string(this->node_number) + "-" + to_string(this->layer_number);
    }

    string LayerNode::toNodeName() const {
        return "node" + to_string(this->node_number);
    }

    bool LayerNode::operator<(const LayerNode& target_layer_node) const {
        if (target_layer_node.getLayerNumber() == this->layer_number) {
            return target_layer_node.getNodeNumber() < this->node_number;
        }
        return target_layer_node.getLayerNumber() < this->layer_number;
    }

    bool LayerNode::operator==(const LayerNode& target_layer_node) const{
        return this->isSameLayerNode(target_layer_node);
    }

    bool LayerNode::operator!=(const LayerNode& target_layer_node) const{
        return !this->isSameLayerNode(target_layer_node);
    }

    string LayerNode::serialize() const {
        json j;

        j["node_number"] = this->node_number;
        j["layer_number"] = this->layer_number;

        return j.dump();
    }

    void LayerNode::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->node_number = j["node_number"];
        this->layer_number = j["layer_number"];
    }

}

