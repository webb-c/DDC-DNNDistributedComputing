/*
 * DNNSublayerTable.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNSublayerTable.h"

namespace inet
{
    
    DNNSublayerTable::DNNSublayerTable() {}

    bool DNNSublayerTable::isRouteIDExist(string string_route_id) {
        if (this->routing_table.count(string_route_id) == 0) {
            return false;
        }
        else {
            return true;
        }
    }

    LayerNode DNNSublayerTable::findNextLayerNodeByRouteID(string string_route_id) {
        if (isRouteIDExist(string_route_id)){
            return this->routing_table[string_route_id];
        }
        else{
            throw runtime_error(string_route_id + "does not exist.");
        }
    }

    void DNNSublayerTable::deleteNextLayerNodeByRouteID(string string_route_id) {
        this->routing_table.erase(string_route_id);
    }

    LayerNode DNNSublayerTable::popNextLayerNodeByRouteID(string string_route_id) {
        if (isRouteIDExist(string_route_id)){
            LayerNode next_layer_node = this->routing_table[string_route_id];
            deleteNextLayerNodeByRouteID(string_route_id);

            return next_layer_node;
        }
        else{
            throw runtime_error(string_route_id + "does not exist.");
        }
    }
    void DNNSublayerTable::registerDNNRoute(string string_route_id, LayerNode next_computing_layer_node) {
        if (!isRouteIDExist(string_route_id)){
            this->routing_table[string_route_id] = next_computing_layer_node;
        }
        else{
            if (this->routing_table[string_route_id] != next_computing_layer_node) {
                throw runtime_error(string_route_id + "already exists. But next_computing_layer_node is different.");
            }
        }
    }
}
