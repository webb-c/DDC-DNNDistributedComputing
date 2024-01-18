/*
 * DNNInformation.cpp
 *
 *  Created on: 2023. 12. 31.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNInformation.h"

namespace inet
{   
    DNNInformation::DNNInformation() : DNNInformation::DNNInformation(DNNID(), LayerNode(), LayerNode()){
    }

    DNNInformation::DNNInformation(DNNID dnn_id, LayerNode src_layer_node, LayerNode dst_layer_node){
        this->dnn_id = dnn_id;
        this->src_layer_node = src_layer_node;
        this->dst_layer_node = dst_layer_node;
    }

    string DNNInformation::serialize() const {
        json j;

        j["dnn_id"] = this->dnn_id.serialize();
        j["src_layer_node"] = this->src_layer_node.serialize();
        j["dst_layer_node"] = this->dst_layer_node.serialize();

        return j.dump();
    }

    void DNNInformation::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->dnn_id.deserialize(j["dnn_id"]);
        this->src_layer_node.deserialize(j["src_layer_node"]);
        this->dst_layer_node.deserialize(j["dst_layer_node"]);
    }

} // namespace dnn
