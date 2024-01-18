/*
 * DNNSublayer.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNSUBLAYER_H
#define __DNN_DNNSUBLAYER_H

#include <iostream>

#include "ddc/dnn/DNNConfig.h"
#include "ddc/dnn/DNNID.h"
#include "ddc/dnn/DNNRouteID.h"
#include "ddc/dnn/DNNOutput.h"

#include "ddc/layernode/LayerNode.h"

namespace inet {

class DNNSublayer{
  private:
    double computing_size;
    DNNOutput output;
    DNNID id;
    DNNRouteID route_id;

    LayerNode computing_layer_node;
    LayerNode next_computing_layer_node;

    bool need_computing = true;

    bool is_source = false;
    bool is_last = false;

  public:
    DNNSublayer();
    DNNSublayer(double computing_size, double link, DNNID id, LayerNode computing_layer_node, LayerNode next_computing_layer_node);

    void disableComputation();
    std::string routeIDToString() const;
    void loadDNNRouteID();
    
    bool isComputingDNNSublayer() const;
    bool isLinkDNNSublayer() const;

    DNNSublayer *deepcopy();

    void setComputingSize(double computing_size) { this->computing_size = computing_size; }
    void setOutput(DNNOutput output) { this->output = output; }
    void setId(DNNID id) { this->id = id; }
    void setComputingLayerNode(LayerNode layer_node) { this->computing_layer_node = layer_node; }
    void setNextComputingLayerNode(LayerNode layer_node) { this->next_computing_layer_node = layer_node; }

    double getComputingSize() const { return this->computing_size; };
    DNNOutput getOutput() const { return this->output; };
    DNNRouteID getDNNRoutID() const { return this->route_id; };
    DNNID getDNNId() const { return this->id; };
    LayerNode getComputingLayerNode() const { return this->computing_layer_node; };
    LayerNode getNextComputingLayerNode() const { return this->next_computing_layer_node; };
    bool getNeedComputing() const { return this->need_computing; };


    bool isSource() { return this->is_source; }
    void setLast();
    bool isLast() { return this->is_last; }


};

}

#endif // ifndef __DNN_DNNSUBLAYER_H
