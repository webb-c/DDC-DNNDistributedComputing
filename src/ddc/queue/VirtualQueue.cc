/*
 * VirtualQueue.cc
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#include "ddc/queue/VirtualQueue.h"

namespace inet{
    VirtualQueue::VirtualQueue(){}

    vector<DNNSublayer> &VirtualQueue::returnVirtualQueue(LayerNodePair layer_node_pair){
        if (!isVirtualQueueExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + " queue does not exist.");
        }
        return this->virtual_queues[layer_node_pair];
    }

    void VirtualQueue::addVirtualQueue(LayerNodePair layer_node_pair){
        vector<DNNSublayer> new_queue;
        this->virtual_queues[layer_node_pair] = new_queue;
    }

    bool VirtualQueue::isVirtualQueueExistByLayerNodePair(LayerNodePair layer_node_pair){
        return this->virtual_queues.count(layer_node_pair) > 0;
    }

    void VirtualQueue::resetVirtualQueue() {
        this->virtual_queues.clear();
    }

    bool VirtualQueue::pushDNNSublayerToVirtualQueueByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair){
        if (isVirtualQueueExistByLayerNodePair(layer_node_pair)){
            this->virtual_queues[layer_node_pair].push_back(dnn_sublayer);
            return true;
        }
        else{
            return false;
        }
    }

    DNNSublayer VirtualQueue::popDNNSublayerToVirtualQueueByLayerNodePair(DNNOutput dnn_output, LayerNodePair layer_node_pair) {
        if (!isVirtualQueueExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + " queue does not exist.");
        }

        auto& queue = virtual_queues[layer_node_pair];
        auto it = std::find_if(queue.begin(), queue.end(),
                               [&dnn_output](const DNNSublayer& sublayer) {
                                   return sublayer.routeIDToString() == dnn_output.routeIDToString();
                               });

        if (it != queue.end()) {
            DNNSublayer found_sublayer = *it;
            queue.erase(it);
            return found_sublayer;
        }
        else {
            throw runtime_error("DNNSublayer with specified DNNID not found in the queue.");
        }
    }

    double VirtualQueue::getQueueLengthByLayerNodePair(LayerNodePair layer_node_pair){
        if (!isVirtualQueueExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + "not exists.");
            return -1;
        }

        auto& queue = virtual_queues[layer_node_pair];

        if (layer_node_pair.isSameLayer()){
            return getLinkQueueLength(queue);
        }
        else if (layer_node_pair.isSameNode()){
            return getComputingQueueLength(queue);
        }
        else {
            return -1;
        }
    }

    double VirtualQueue::getComputingQueueLength(const vector<DNNSublayer>& queue){
        double queue_length = 0.0;
        for (const auto& dnn_sub_layer : queue) {
            queue_length += dnn_sub_layer.getComputingSize();
        }
        return queue_length;
    }

    double VirtualQueue::getLinkQueueLength(const vector<DNNSublayer>& queue){
        double queue_length = 0.0;
        for (const auto& dnn_sub_layer : queue) {
            queue_length += dnn_sub_layer.getOutput().getLinkSize();
        }
        return queue_length;
    }
}
