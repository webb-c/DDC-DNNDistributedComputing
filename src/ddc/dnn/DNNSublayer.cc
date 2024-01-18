/*
 * DNNSublayer.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNSublayer.h"

namespace inet
{
    DNNSublayer::DNNSublayer() : DNNSublayer::DNNSublayer(1.0, 1.0, DNNID(), LayerNode(), LayerNode()) {}
    DNNSublayer::DNNSublayer(double computing_size, double link, DNNID id, LayerNode computing_layer_node, LayerNode next_computing_layer_node){
        DNNOutput output = DNNOutput(link, id, computing_layer_node, next_computing_layer_node);
        this->computing_size = computing_size;
        this->output = output;
        this->id = id;
        this->computing_layer_node = computing_layer_node;
        this->next_computing_layer_node = next_computing_layer_node;

        if (output.getLinkSize() == 0.0) {
            throw runtime_error(output.routeIDToString());
        }

        loadDNNRouteID();
    }

    void DNNSublayer::loadDNNRouteID(){
        this->route_id = DNNRouteID(this->id, this->computing_layer_node);
    }

    void DNNSublayer::disableComputation(){
        this->need_computing = false;
    }

    std::string DNNSublayer::routeIDToString() const {
        return this->route_id.toString();
    }

    DNNSublayer *DNNSublayer::deepcopy() {
        DNNSublayer *new_dnn_sublayer = new DNNSublayer(this->computing_size, this->output.getLinkSize(), this->id, this->computing_layer_node, this->next_computing_layer_node);

        if (!this->need_computing) {
            new_dnn_sublayer->disableComputation();
        }
        if (this->isLast()) {
            new_dnn_sublayer->setLast();
        }

        return new_dnn_sublayer;
    }

    bool DNNSublayer::isComputingDNNSublayer() const {
        if (this->computing_layer_node.isSameNode(next_computing_layer_node) && !this->computing_layer_node.isSameLayer(next_computing_layer_node)) {
            return true;
        }

        return false;
    }

    bool DNNSublayer::isLinkDNNSublayer() const {
        if (!this->computing_layer_node.isSameNode(next_computing_layer_node) && this->computing_layer_node.isSameLayer(next_computing_layer_node)) {
            return true;
        }

        return false;
    }

    void DNNSublayer::setLast() {
        this->is_last = true;
        this->output.setLast();
    }
}
