/*
 * DNNSublayerTable.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNSUBLAYERTABLE_H
#define __DNN_DNNSUBLAYERTABLE_H

#include <iostream>
#include <omnetpp.h>
#include <unordered_map>

#include "ddc/dnn/DNNConfig.h"
#include "ddc/dnn/DNNID.h"
#include "ddc/dnn/DNNOutput.h"
#include "ddc/dnn/DNNRouteID.h"

#include "ddc/layernode/LayerNode.h"

namespace inet {

class DNNSublayerTable{
  private:
    map<string, LayerNode> routing_table;

  public:
    DNNSublayerTable();

    map<string, LayerNode> getRoutingTable() { return this->routing_table; }

    bool isRouteIDExist(string string_route_id);
    LayerNode findNextLayerNodeByRouteID(string string_route_id);
    void deleteNextLayerNodeByRouteID(string string_route_id);
    void registerDNNRoute(string string_route_id, LayerNode next_computing_layer_node);
    LayerNode popNextLayerNodeByRouteID(string string_route_id);
};

}

#endif // ifndef __DNN_DNNSUBLAYERTABLE_H
