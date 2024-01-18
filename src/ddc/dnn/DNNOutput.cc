/*
 * DNNOutput.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNOutput.h"

namespace inet{
    DNNOutput::DNNOutput() : DNNOutput::DNNOutput(1.0, DNNID(), LayerNode(), LayerNode()) {
    }
    DNNOutput::DNNOutput(double link, DNNID id, LayerNode computing_layer_node, LayerNode next_computing_layer_node) {
        this->link = link;
        this->dnn_id = id;
        this->computing_layer_node = computing_layer_node;
        this->next_computing_layer_node = next_computing_layer_node;

        loadDNNRouteID();
        initData();
    }

    DNNOutput *DNNOutput::deepcopy() {
        DNNOutput *new_dnn_output = new DNNOutput(this->link, this->dnn_id, this->computing_layer_node, this->next_computing_layer_node);

        if (this->is_last) {
            new_dnn_output->setLast();
        }

        return new_dnn_output;
    }

    void DNNOutput::loadDNNRouteID(){
        this->route_id = DNNRouteID(this->dnn_id, this->next_computing_layer_node);
    }

    void DNNOutput::initData() {
        string temp((int)this->link, '0');
        this->data = temp;
    }

    string DNNOutput::routeIDToString() const {
        return this->route_id.toString();
    }

    double DNNOutput::getEndToEndLatency(double end_simulation_time) const {
        return end_simulation_time - this->dnn_id.getStartSimulationTime();
    }

    string DNNOutput::serialize() const {
        json j;

        j["link"] = this->link;
        j["route_id"] = this->route_id.serialize();
        j["dnn_id"] = this->dnn_id.serialize();
        j["computing_layer_node"] = this->computing_layer_node.serialize();
        j["next_computing_layer_node"] = this->next_computing_layer_node.serialize();
        j["data"] = this->data;
        j["is_last"] = this->is_last;


        return j.dump();
    }

    void DNNOutput::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->link = j["link"];
        this->route_id.deserialize(j["route_id"]);
        this->dnn_id.deserialize(j["dnn_id"]);
        this->computing_layer_node.deserialize(j["computing_layer_node"]);
        this->next_computing_layer_node.deserialize(j["next_computing_layer_node"]);
        this->data = j["data"];
        this->is_last = j["is_last"];
    }
}
