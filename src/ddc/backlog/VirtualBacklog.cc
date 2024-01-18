/*
 * VirtualBacklog.cc
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#include "ddc/backlog/VirtualBacklog.h"

namespace inet{
    VirtualBacklog::VirtualBacklog(){}

    vector<DNNSublayer> &VirtualBacklog::returnVirtualBacklog(LayerNodePair layer_node_pair){
        if (!isVirtualBacklogExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + " backlog does not exist.");
        }
        return this->virtual_backlogs[layer_node_pair];
    }

    void VirtualBacklog::addVirtualBacklog(LayerNodePair layer_node_pair){
        vector<DNNSublayer> new_backlog;
        this->virtual_backlogs[layer_node_pair] = new_backlog;
    }

    bool VirtualBacklog::isVirtualBacklogExistByLayerNodePair(LayerNodePair layer_node_pair){
        return this->virtual_backlogs.count(layer_node_pair) > 0;
    }

    void VirtualBacklog::resetVirtualBacklog() {
//        for (auto &pair : this->virtual_backlogs) {
//            this->virtual_backlogs[pair.first].clear();
//        }
        this->virtual_backlogs.clear();
    }

    bool VirtualBacklog::pushDNNSublayerToVirtualBacklogByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair){
        if (isVirtualBacklogExistByLayerNodePair(layer_node_pair)){
            this->virtual_backlogs[layer_node_pair].push_back(dnn_sublayer);
            return true;
        }
        else{
            return false;
        }
    }

    DNNSublayer VirtualBacklog::popDNNSublayerToVirtualBacklogByLayerNodePair(DNNOutput dnn_output, LayerNodePair layer_node_pair) {
        if (!isVirtualBacklogExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + " backlog does not exist.");
        }

        auto& backlog = virtual_backlogs[layer_node_pair];
        auto it = std::find_if(backlog.begin(), backlog.end(),
                               [&dnn_output](const DNNSublayer& sublayer) {
                                   return sublayer.routeIDToString() == dnn_output.routeIDToString();
                               });

        if (it != backlog.end()) {
            DNNSublayer found_sublayer = *it;
            backlog.erase(it);
            return found_sublayer;
        }
        else {
            throw runtime_error("DNNSublayer with specified DNNID not found in the backlog.");
        }
    }

    double VirtualBacklog::getBacklogLengthByLayerNodePair(LayerNodePair layer_node_pair){
        if (!isVirtualBacklogExistByLayerNodePair(layer_node_pair)){
            throw runtime_error(layer_node_pair.toString() + "not exists.");
            return -1;
        }

        auto& backlog = virtual_backlogs[layer_node_pair];

        if (layer_node_pair.isSameLayer()){
            return getLinkBacklogLength(backlog);
        }
        else if (layer_node_pair.isSameNode()){
            return getComputingBacklogLength(backlog);
        }
        else {
            return -1;
        }
    }

    double VirtualBacklog::getComputingBacklogLength(const vector<DNNSublayer>& backlog){
        double backlog_length = 0.0;
        for (const auto& dnn_sub_layer : backlog) {
            backlog_length += dnn_sub_layer.getComputingSize();
        }
        return backlog_length;
    }

    double VirtualBacklog::getLinkBacklogLength(const vector<DNNSublayer>& backlog){
        double backlog_length = 0.0;
        for (const auto& dnn_sub_layer : backlog) {
            backlog_length += dnn_sub_layer.getOutput().getLinkSize();
        }
        return backlog_length;
    }
}
