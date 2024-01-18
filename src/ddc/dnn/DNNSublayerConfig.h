/*
 * DNNSublayerConfig.h
 *
 *  Created on: 2023. 12. 29.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNSUBLAYERCONFIG_H
#define __DNN_DNNSUBLAYERCONFIG_H

#include <iostream>
#include <omnetpp.h>

#include "ddc/libs/nlohmann/json.hpp"

#include "ddc/dnn/DNNID.h"
#include "ddc/layernode/LayerNode.h"

using json = nlohmann::json;

namespace inet{

class DNNSublayerConfig
{
  private:
    string model_name;
    int sublayer_index;
    bool need_computing;
    DNNID dnn_id;
    LayerNode computing_layer_node;
    LayerNode next_computing_layer_node;
    bool is_source = false;
    bool is_last = false;

  public:
    DNNSublayerConfig();
    DNNSublayerConfig(std::string model_name, int sublayer_index, bool need_computing, DNNID dnn_id, LayerNode computing_layer_node, LayerNode next_computing_layer_node);

    std::string getModelName() { return this->model_name; }
    int getSublayerIndex() { return this->sublayer_index; }
    bool getNeedComputing() { return this->need_computing; }
    DNNID getDNNID() { return this->dnn_id; }
    LayerNode getComputingLayerNode() const { return this->computing_layer_node; };
    LayerNode getNextComputingLayerNode() const { return this->next_computing_layer_node; };

    bool isSource() { return this->is_source; }
    void setSource() { this->is_source = true; }

    bool isLast() { return this->is_last; }
    void setLast() { this->is_last = true; }

    string serialize() const;
    void deserialize(string json_string);

    string IDToString() const { return this->dnn_id.toString(); }
};

}

#endif // ifndef __DNN_DNNSUBLAYERCONFIG_H
