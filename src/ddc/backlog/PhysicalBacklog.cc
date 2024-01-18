/*
 * PhysicalBacklog.cc
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#include "ddc/backlog/PhysicalBacklog.h"

namespace inet{
    PhysicalBacklog::PhysicalBacklog(){}

    queue<DNNSublayer> &PhysicalBacklog::returnPhysicalBacklog(LayerNodePair layer_node_pair){
        if (!isPhysicalBacklogExistByLayerNodePair(layer_node_pair)){
            throw std::runtime_error("Backlog does not exist for the given LayerNodePair.");
        }
        return this->physical_backlogs[layer_node_pair];
    }

    void PhysicalBacklog::addPhysicalBacklog(LayerNodePair layer_node_pair){
        queue<DNNSublayer> new_backlog;
        this->physical_backlogs[layer_node_pair] = new_backlog;
    }

    bool PhysicalBacklog::isPhysicalBacklogExistByLayerNodePair(LayerNodePair layer_node_pair){
        return physical_backlogs.find(layer_node_pair) != physical_backlogs.end();
    }

    bool PhysicalBacklog::pushDNNSublayerToPhysicalBacklogByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair){
        if (isPhysicalBacklogExistByLayerNodePair(layer_node_pair)){
            this->physical_backlogs[layer_node_pair].push(dnn_sublayer);
            return true;
        }
        else{
            return false;
        }
    }

    DNNSublayer PhysicalBacklog::popDNNSublayerToPhysicalBacklogByLayerNodePair(LayerNodePair layer_node_pair){
        if (!isPhysicalBacklogExistByLayerNodePair(layer_node_pair)){
            throw std::runtime_error("Backlog does not exist for the given LayerNodePair.");
        }

        auto& queue = this->physical_backlogs[layer_node_pair];
        if (queue.empty()) {
            throw std::runtime_error("Backlog is empty.");
        }

        DNNSublayer front_element = queue.front();
        queue.pop();
        return front_element;
    }

    bool PhysicalBacklog::isPhysicalBacklogEmpty(LayerNodePair layer_node_pair){
        auto& queue = this->physical_backlogs[layer_node_pair];
        return queue.empty();
    }

    double PhysicalBacklog::getBacklogLengthByLayerNodePair(LayerNodePair layer_node_pair){
        queue<DNNSublayer> &physical_backlog = returnPhysicalBacklog(layer_node_pair);

        // layer가 같으면 데이터를 보내므로 link를 더함
        if (layer_node_pair.isSameLayer()){
            return getLinkBacklogLength(physical_backlog);
        }
        // node가 같으면 데이터를 보내므로 link를 더함
        else if (layer_node_pair.isSameNode()){
            return getComputingBacklogLength(physical_backlog);
        }

        return -1;
    }

    double PhysicalBacklog::getComputingBacklogLength(queue<DNNSublayer> &physical_backlog){
        double backlog_length = 0.0;
        queue<DNNSublayer> temp_queue = physical_backlog;

        while (!temp_queue.empty()) {
            DNNSublayer dnn_sub_layer = temp_queue.front();
            backlog_length += dnn_sub_layer.getComputingSize();
            temp_queue.pop();
        }

        return backlog_length;
    }

    double PhysicalBacklog::getLinkBacklogLength(queue<DNNSublayer> &physical_backlog){
        double backlog_length = 0.0;
        queue<DNNSublayer> temp_queue = physical_backlog;

        while (!temp_queue.empty()) {
            DNNSublayer dnn_sub_layer = temp_queue.front();
            backlog_length += dnn_sub_layer.getOutput().getLinkSize();
            temp_queue.pop();
        }

        return backlog_length;
    }
}
