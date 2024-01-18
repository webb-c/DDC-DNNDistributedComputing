/*
 * DNNRouteID.cpp
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNRouteID.h"

namespace inet
{   
    DNNRouteID::DNNRouteID() {}
    DNNRouteID::DNNRouteID(DNNID dnn_id, LayerNode layer_node){
        this->dnn_id = dnn_id;
        this->layer_node = layer_node;
    }

    string DNNRouteID::toString() const{
        return "dnn_id:(" + this->dnn_id.toString() + "), destination_layer_node:" + this->layer_node.toString();
    }

    string DNNRouteID::serialize() const {
        json j;

        j["dnn_id"] = this->dnn_id.serialize();
        j["layer_node"] = this->layer_node.serialize();

        return j.dump();
    }
    void DNNRouteID::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->dnn_id.deserialize(j["dnn_id"]);
        this->layer_node.deserialize(j["layer_node"]);
    }

} // namespace dnn
