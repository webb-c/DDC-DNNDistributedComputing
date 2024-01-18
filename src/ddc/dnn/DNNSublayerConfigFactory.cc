/*
 * DNNSublayerConfigFactory.cc
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNSublayerConfigFactory.h"

namespace inet{

    DNNSublayerConfigFactory::DNNSublayerConfigFactory() {};

    void DNNSublayerConfigFactory::resetDNNSublayerConfigVector() {
        this->dnn_sublayer_config_vector.clear();
    }

    vector<DNNSublayerConfig *> DNNSublayerConfigFactory::makeDNNSublayerConfigVector(DNNInformation *dnn_information, LayerNodePairPath &layer_node_pair_path) {
        resetDNNSublayerConfigVector();

        string model_name = dnn_information->getDNNID().getDNNModelName();
        int sublayer_index = 0;
        DNNID dnn_id = dnn_information->getDNNID();

        string temp = "";

        for (int i = 0; i < layer_node_pair_path.getPathLength(); i++){
            LayerNodePair layer_node_pair = layer_node_pair_path.next();

            LayerNode computing_layer_node = layer_node_pair.getSrcLayerNode();
            LayerNode next_computing_layer_node = layer_node_pair.getDstLayerNode();

            if (layer_node_pair.isSameNode() && !layer_node_pair.isSameLayer() && i==0){ // source is computing first dnn sublayer, don't need to send data.
                sublayer_index += 1;
            }

            if (layer_node_pair.isSameLayer() && !layer_node_pair.isSameNode()) { // need computing = false
                DNNSublayerConfig *dnn_sublayer_config = new DNNSublayerConfig(model_name, sublayer_index, false, dnn_id, computing_layer_node, next_computing_layer_node);
                this->dnn_sublayer_config_vector.push_back(dnn_sublayer_config);
            }
            else if (layer_node_pair.isSameNode() && !layer_node_pair.isSameLayer()) { // need computing = true
                DNNSublayerConfig *dnn_sublayer_config = new DNNSublayerConfig(model_name, sublayer_index, true, dnn_id, computing_layer_node, next_computing_layer_node);
                this->dnn_sublayer_config_vector.push_back(dnn_sublayer_config);
            }

            if (layer_node_pair.isSameNode() && !layer_node_pair.isSameLayer()){ // computing link
                sublayer_index += 1;
            }

        }
        this->dnn_sublayer_config_vector[0]->setSource();
        this->dnn_sublayer_config_vector.back()->setLast();

//        string temp="";
//        for (DNNSublayerConfig *dnn_sublayer_config : this->dnn_sublayer_config_vector) {
//            temp += dnn_sublayer_config->serialize() + "/\n";
//        }

        return this->dnn_sublayer_config_vector;
    }


}

