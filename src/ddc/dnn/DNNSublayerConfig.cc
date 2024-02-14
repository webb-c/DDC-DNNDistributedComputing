/*
 * DNNSublayerConfig.cc
 *
 *  Created on: 2023. 12. 29.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNSublayerConfig.h"

namespace inet{

    DNNSublayerConfig::DNNSublayerConfig(){}
    DNNSublayerConfig::DNNSublayerConfig(string model_name, double input_size, int sublayer_index, bool need_computing, DNNID dnn_id, LayerNode computing_layer_node, LayerNode next_computing_layer_node){
        this->model_name = model_name;
        this->input_size = input_size;
        this->sublayer_index = sublayer_index;
        this->need_computing = need_computing;
        this->dnn_id = dnn_id;
        this->computing_layer_node = computing_layer_node;
        this->next_computing_layer_node = next_computing_layer_node;
    }

    string DNNSublayerConfig::serialize() const {
        json j;

        j["dnn_id"] = this->dnn_id.serialize();
        j["computing_layer_node"] = this->computing_layer_node.serialize();
        j["next_computing_layer_node"] = this->next_computing_layer_node.serialize();
        j["model_name"] = this->model_name;
        j["input_size"] = this->input_size;
        j["sublayer_index"] = this->sublayer_index;
        j["need_computing"] = this->need_computing;
        j["is_source"] = this->is_source;
        j["is_last"] = this->is_last;

        return j.dump();
    }

    void DNNSublayerConfig::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->dnn_id.deserialize(j["dnn_id"]);
        this->computing_layer_node.deserialize(j["computing_layer_node"]);
        this->next_computing_layer_node.deserialize(j["next_computing_layer_node"]);
        this->model_name = j["model_name"];
        this->input_size = j["input_size"];
        this->sublayer_index = j["sublayer_index"];
        this->need_computing = j["need_computing"];
        this->is_source = j["is_source"];
        this->is_last = j["is_last"];
    }
}

