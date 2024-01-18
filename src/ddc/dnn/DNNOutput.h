/*
 * DNNOutput.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNOUTPUT_H
#define __DNN_DNNOUTPUT_H

#include <iostream>

#include "ddc/libs/nlohmann/json.hpp"

#include "ddc/dnn/DNNID.h"
#include "ddc/dnn/DNNRouteID.h"

#include "ddc/layernode/LayerNodePair.h"

using json = nlohmann::json;

namespace inet {

class DNNOutput{
  private:
    double link;
    DNNRouteID route_id;
    DNNID dnn_id;
    LayerNode computing_layer_node;
    LayerNode next_computing_layer_node; // LayerNode of DNNOutput destination.
    string data;
    bool is_last = false;
    
  public:
    DNNOutput();
    DNNOutput(double link, DNNID id, LayerNode computing_layer_node, LayerNode next_computing_layer_node);

    void loadDNNRouteID();
    string routeIDToString() const;
    string IDToString() const { return this->dnn_id.toString(); }
    void initData();

    DNNOutput *deepcopy();

    void setLast() { this->is_last = true; }
    bool isLast() const { return this->is_last; }

    DNNID getDNNID() { return this->dnn_id; }
    DNNRouteID getDNNRoutID() { return this->route_id; }
    double getLinkSize() { return this->link; }
    LayerNode getComputingLayerNode() const { return this->computing_layer_node; }
    LayerNode getNextComputingLayerNode() const { return this->next_computing_layer_node; }

    double getEndToEndLatency(double end_simulation_time) const;

    string serialize() const;
    void deserialize(string json_string);
};

}

#endif // ifndef __DNN_DNNOUTPUT_H
