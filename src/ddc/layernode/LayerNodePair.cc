/*
 * LayerNodePair.cc
 *
 *  Created on: 2023. 12. 28.
 *      Author: Suhwan
 */

#include "ddc/layernode/LayerNodePair.h"

namespace inet {

    LayerNodePair::LayerNodePair() : LayerNodePair(LayerNode(), LayerNode()) {}

    LayerNodePair::LayerNodePair(LayerNode src_layer_node,  LayerNode dst_layer_node){
        this->src_layer_node = src_layer_node;
        this->dst_layer_node = dst_layer_node;
    }

    string LayerNodePair::toString() const {
        return "LayerNodePair(" + this->src_layer_node.toString() + ", " + this->dst_layer_node.toString() + ")";
    }

    LayerNode LayerNodePair::getSrcLayerNode() const{
        return this->src_layer_node;
    }

    LayerNode LayerNodePair::getDstLayerNode() const{
        return this->dst_layer_node;
    }

    bool LayerNodePair::operator<(const LayerNodePair& target_layer_node_pair) const {
        if (this->src_layer_node == target_layer_node_pair.getSrcLayerNode()){
            return this->dst_layer_node < target_layer_node_pair.getDstLayerNode();
        }
        return this->src_layer_node < target_layer_node_pair.getSrcLayerNode();
    }

    bool LayerNodePair::operator==(const LayerNodePair& target_layer_node_pair) const {
        return this->src_layer_node == target_layer_node_pair.getSrcLayerNode() &&
               this->dst_layer_node == target_layer_node_pair.getDstLayerNode();
    }

    bool LayerNodePair::isSameLayer(){
        return src_layer_node.isSameLayer(dst_layer_node);
    }

    bool LayerNodePair::isSameNode(){
        return src_layer_node.isSameNode(dst_layer_node);
    }

    string LayerNodePair::serialize() const {
        json j;

        j["src_layer_node"] = this->src_layer_node.serialize();
        j["dst_layer_node"] = this->dst_layer_node.serialize();

        return j.dump();
    }

    void LayerNodePair::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->src_layer_node.deserialize(j["src_layer_node"]);
        this->dst_layer_node.deserialize(j["dst_layer_node"]);
    }

}

