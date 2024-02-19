/*
 * DNNSublayerFactory.cc
 *
 *  Created on: 2023. 12. 29.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNSublayerFactory.h"

namespace inet{

    DNNSublayerFactory::DNNSublayerFactory() {};

    DNNSublayer DNNSublayerFactory::makeDNNSublayer(DNNSublayerConfig dnn_sublayer_config){
        unzipDNNSublayerConfig(dnn_sublayer_config);
        includeDNNConfig();
        loadLinkAndComputing();
        buildDNNSublayer();

        return this->dnn_sublayer;
    }

    void DNNSublayerFactory::unzipDNNSublayerConfig(DNNSublayerConfig dnn_sublayer_config){
        this->input_size = dnn_sublayer_config.getInputSize();
        this->model_name = dnn_sublayer_config.getModelName();
        this->sublayer_index = dnn_sublayer_config.getSublayerIndex();
        this->need_computing = dnn_sublayer_config.getNeedComputing();
        this->dnn_id = dnn_sublayer_config.getDNNID();
        this->computing_layer_node = dnn_sublayer_config.getComputingLayerNode();
        this->next_computing_layer_node = dnn_sublayer_config.getNextComputingLayerNode();
        this->is_source = dnn_sublayer_config.isSource();
        this->is_last = dnn_sublayer_config.isLast();
    }

    void DNNSublayerFactory::includeDNNConfig(){
        this->dnn_config = this->dnn_config_factory.makeDNNConfig(this->model_name, this->input_size);
    }

    void DNNSublayerFactory::loadLinkAndComputing(){
        loadLink();
        loadComputing();
    }

    void DNNSublayerFactory::loadLink(){
        this->link = this->dnn_config.getLinkByLayerIndex(this->sublayer_index);
    }

    void DNNSublayerFactory::loadComputing(){
        this->computing = this->dnn_config.getComputingByLayerIndex(this->sublayer_index);
    }

    void DNNSublayerFactory::buildDNNSublayer(){
        this->dnn_sublayer = DNNSublayer(this->computing, this->link, this->dnn_id, this->computing_layer_node, this->next_computing_layer_node);

        if (!this->need_computing){
            this->dnn_sublayer.disableComputation();
        }
        this->dnn_sublayer.setLast();
    }


}

