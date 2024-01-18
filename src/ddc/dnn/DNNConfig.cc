/*
 * DNNConfig.cc
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNConfig.h"

namespace inet{

    DNNConfig::DNNConfig() : DNNConfig::DNNConfig(createDefaultJson()){}

    DNNConfig::DNNConfig(json json_config){
        this->name = json_config["name"];
        this->sublayer_num = json_config["sublayer_num"];
        this->link_and_computing_config = json_config["config"];
        this->link_config_vector = this->link_and_computing_config[LINK_INDEX];
        this->computing_config_vector = this->link_and_computing_config[COMPUTING_INDEX];
    }

    json DNNConfig::createDefaultJson(){
        return {
            {"name", "model_1"},
            {"sublayer_num", 6},
            {"config", {{9.41, 50.18, 12.54, 1.54, 0.77, 0.04}, {0, 3.81, 20.08, 1.20, 0.07, 0.002}}}
        };
    }

    double DNNConfig::getLinkByLayerIndex(int layer_index){
        return this->link_config_vector[layer_index];
    }

    double DNNConfig::getComputingByLayerIndex(int layer_index){
        return this->computing_config_vector[layer_index];
    }

}

