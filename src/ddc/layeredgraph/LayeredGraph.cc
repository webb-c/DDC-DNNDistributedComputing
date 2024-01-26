/*
 * LayeredGraph.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#include "ddc/layeredgraph/LayeredGraph.h"

namespace inet
{
    LayeredGraph::LayeredGraph() : LayeredGraph(-1, -1, new Dijkstra()) {}
    LayeredGraph::LayeredGraph(int graph_depth, int node_num, AlgorithmBase *algorithm) {
        this->graph_depth = graph_depth;
        this->node_num = node_num;
        this->algorithm = algorithm;
        resetNodeSyncIndicator();
    }
    
    ComputeDNN *LayeredGraph::returnComputeDNNApplication(cModule *node) {
        ComputeDNN* compute_dnn_application = dynamic_cast<ComputeDNN *>(node->getSubmodule("app", 0));
        return compute_dnn_application;
    }

    void LayeredGraph::registerNode(cModule *node) {
        this->node = node;
        addNode();
        addComputeDNNApplication();
        addLayerNodesFromNode();
        addLayerNodeNodeTable();
        addLayerNodeApplicationTable();
        addNodeCapacityTable();

        this->link_job_nums.push_back(0);
        this->compute_job_nums.push_back(0);
    }

    // nodes
    void LayeredGraph::addNode() {
        this->nodes.push_back(this->node);
    }

    // nodes_application
    void LayeredGraph::addComputeDNNApplication() {
        ComputeDNN *compute_dnn = returnComputeDNNApplication(this->node);

        this->nodes_application.push_back(compute_dnn);
    }

    // layer_nodes
    void LayeredGraph::addLayerNodesFromNode() {
        int node_num = returnNodeNumberFromNode(this->node);

        for (int i = 1; i < this->graph_depth + 1; i++){
            LayerNode layer_node = LayerNode(node_num, i);
            addLayerNode(layer_node);
        }

    }

    void LayeredGraph::addLayerNode(LayerNode layer_node) {
        this->layer_nodes.push_back(layer_node);
    }

    // layer_node_node_table
    void LayeredGraph::addLayerNodeNodeTable() {
        int node_num = returnNodeNumberFromNode(this->node);

        for (int i = 1; i < this->graph_depth + 1; i++){
            LayerNode layer_node = LayerNode(node_num, i);

            this->layer_node_node_table[layer_node] = this->node;
        }
    }

    // lyaer_node_application_table
    void LayeredGraph::addLayerNodeApplicationTable() {
        int node_num = returnNodeNumberFromNode(this->node);

        for (int i = 1; i < this->graph_depth + 1; i++){
            LayerNode layer_node = LayerNode(node_num, i);

            this->layer_node_node_table[layer_node] = returnComputeDNNApplication(this->node);
        }
    }

    void LayeredGraph::addNodeCapacityTable() {
        int node_num = returnNodeNumberFromNode(this->node);

        ComputeDNN *compute_dnn = returnComputeDNNApplication(this->node);

        this->node_link_capacity_table[node_num] = compute_dnn->getBitrate();
        this->node_compute_capacity_table[node_num] = compute_dnn->getComputeCapacity();
    }

    void LayeredGraph::initNodeSyncIndicator() {
        resetNodeSyncIndicator();
    }

    void LayeredGraph::resetNodeSyncIndicator() {
        for (int i = 1; i < this->node_num + 1; i++) {
            string node_name = "node" + to_string(i);
            this->node_sync_indicator[node_name] = false;
        }
    }

    void LayeredGraph::buildAdjacencyMatrix() {
        // for non-outbound link nodes
        for (const LayerNode &layer_node : this->layer_nodes) {
            this->adjacency_matrix[layer_node] = vector<LayerNode>();
        }

        for (cModule *node : this->nodes){
            ComputeDNN *compute_dnn_application = returnComputeDNNApplication(node);
            vector<LayerNodePair> links = compute_dnn_application->getLinks();

            for (LayerNodePair link : links) {
                LayerNode src_layer_node = link.getSrcLayerNode();
                LayerNode dst_layer_node = link.getDstLayerNode();

                this->adjacency_matrix[src_layer_node].push_back(dst_layer_node);
            }
        }
    }

    void LayeredGraph::initLayeredGraph() {
        this->layered_graph.clear();
    }

    void LayeredGraph::initRecorder() {
        // init virtual queue recorder of all layer nodes
        for (const auto &pair : this->adjacency_matrix) {
            LayerNode src = pair.first;
            vector<LayerNode> adjacency = pair.second;

            for (LayerNode &layer_node : adjacency) {
                LayerNodePair virtual_link = LayerNodePair(src, layer_node);

                cOutVector *layer_node_virtual_queue_recorder = new cOutVector();
                layer_node_virtual_queue_recorder->setName(("2. layer node virtual backlog: " + virtual_link.toString()).c_str());

                this->layer_node_virtual_queue_recorder_table[virtual_link] = layer_node_virtual_queue_recorder;

                string recorder_name = virtual_link.getSrcLayerNode().toNodeName() + "->" + virtual_link.getDstLayerNode().toNodeName();
                cOutVector *node_virtual_queue_recorder = new cOutVector();
                node_virtual_queue_recorder->setName(("1. node virtual backlog: " + recorder_name).c_str());

                if (this->node_virtual_queue_recorder_table.count(recorder_name) == 0) {
                    this->node_virtual_queue_recorder_table[recorder_name] = node_virtual_queue_recorder;
                }
            }
        }
    }

    void LayeredGraph::sync(NodeInformation node_information){
        string node_name = node_information.getNodeName();
        map<LayerNodePair, double> links_information = node_information.getLinksInformation();

        if (links_information.size() == 0) { throw runtime_error(node_information.serialize()); }

        for (auto &e : links_information) {
            this->layered_graph[e.first] = links_information[e.first];
        }

        this->node_sync_indicator[node_name] = true;
    }

    void LayeredGraph::update() {
        double current_update_time = simTime().dbl();

        for (int i = 0; i < this->node_num; i++) {
            this->link_job_nums[i] = 0;
            this->compute_job_nums[i] = 0;
        }

        for (const auto &pair : this->layered_graph) {
            LayerNodePair link = pair.first;
            LayerNode src = link.getSrcLayerNode();
            double link_backlog = pair.second;

            if (link.isSameLayer() && link_backlog > 0) {
                link_job_nums[src.getNodeNumber()]++;
            }
            else if (link.isSameNode() && link_backlog > 0) {
                compute_job_nums[src.getNodeNumber()]++;
            }
        }

        // updateLink(link_job_nums);
        // updateCompute(compute_job_nums);

        for (const auto &pair : this->node_link_capacity_table) {
            int node_num = pair.first;
            double link_capacity = pair.second;

            int link_job_num = link_job_nums[node_num];

            if (link_job_num == 0) {
                continue;
            }

            double time_delta = (current_update_time - this->latest_update_time > 0) ? current_update_time - this->latest_update_time : 0;
            double link_delta = (link_capacity / link_job_num) * time_delta;

            for (const auto &pair : this->layered_graph) {
                LayerNodePair link = pair.first;
                LayerNode src = link.getSrcLayerNode();
                double link_backlog = pair.second;

                if (node_num == src.getNodeNumber() && link.isSameLayer()) {
                    this->layered_graph[link] = (link_backlog - link_delta <= 0) ? 0 : link_backlog - link_delta;
                }
            }
        }

        for (const auto &pair : this->node_compute_capacity_table) {
            int node_num = pair.first;
            double compute_capacity = pair.second;

            int compute_job_num = compute_job_nums[node_num];

            if (compute_job_num == 0) {
                continue;
            }

            double time_delta = (current_update_time - this->latest_update_time > 0) ? current_update_time - this->latest_update_time : 0;
            double compute_delta = (compute_capacity / compute_job_num) * time_delta;

            for (const auto &pair : this->layered_graph) {
                LayerNodePair link = pair.first;
                LayerNode src = link.getSrcLayerNode();
                double compute_backlog = pair.second;

                if (node_num == src.getNodeNumber() && link.isSameNode()) {
                    this->layered_graph[link] = (compute_backlog - compute_delta <= 0) ? 0 : compute_backlog - compute_delta;
                }
            }
        }

        this->latest_update_time = current_update_time;
    }

    void LayeredGraph::updateLink(vector<int> &link_job_nums) {
        for (const auto &pair : this->node_link_capacity_table) {
            int node_num = pair.first;
            double link_capacity = pair.second;

            int link_job_num = link_job_nums[node_num];

            if (link_job_num == 0) {
                continue;
            }

            double link_delta = (link_capacity / link_job_num) * 0.1;

            for (const auto &pair : this->layered_graph) {
                LayerNodePair link = pair.first;
                LayerNode src = link.getSrcLayerNode();
                double link_backlog = pair.second;

                if (node_num == src.getNodeNumber() && link.isSameLayer()) {
                    this->layered_graph[link] = (link_backlog - link_delta <= 0) ? 0 : link_backlog - link_delta;
                }
            }
        }
    }

    void LayeredGraph::updateCompute(vector<int> &compute_job_nums) {
        for (const auto &pair : this->node_compute_capacity_table) {
            int node_num = pair.first;
            double compute_capacity = pair.second;

            int compute_job_num = compute_job_nums[node_num];

            if (compute_job_num == 0) {
                continue;
            }

            double compute_delta = (compute_capacity / compute_job_num) * 0.1;

            for (const auto &pair : this->layered_graph) {
                LayerNodePair link = pair.first;
                LayerNode src = link.getSrcLayerNode();
                double compute_backlog = pair.second;

                if (node_num == src.getNodeNumber() && link.isSameNode()) {
                    this->layered_graph[link] = (compute_backlog - compute_delta <= 0) ? 0 : compute_backlog - compute_delta;
                }
            }
        }
    }

    vector<LayerNode> LayeredGraph::findPath(LayerNode src_layer_node, LayerNode dst_layer_node) {
        return this->algorithm->findPath(this->layered_graph, this->adjacency_matrix, this->layer_nodes, src_layer_node, dst_layer_node);
    }

    double LayeredGraph::getArrivalRate(LayerNode src_layer_node, LayerNode dst_layer_node) {
        vector<LayerNode> path = findPath(src_layer_node, dst_layer_node);
        LayerNodePairPath pair_path = LayerNodePairPath(path);

        double arrival_rate = 0.0;

        for (int i = 0; i < pair_path.getPathLength(); i++) {
            LayerNodePair layer_node_pair = pair_path.next();
            arrival_rate += this->layered_graph[layer_node_pair];
        }

        return arrival_rate;
    }

    void LayeredGraph::applyVirtualUpdate(vector<DNNSublayer> &dnn_sublayer_vector) {
        for (const DNNSublayer& dnn_sublayer : dnn_sublayer_vector) {
            LayerNode src_layer_node = dnn_sublayer.getComputingLayerNode();
            LayerNode dst_layer_node = dnn_sublayer.getNextComputingLayerNode();
            LayerNodePair link = LayerNodePair(src_layer_node, dst_layer_node);

            if (this->layered_graph.count(link) == 0) {
                throw runtime_error(link.toString() + "is not exists");
            }

            if (dnn_sublayer.isComputingDNNSublayer()) {
                this->layered_graph[link] += dnn_sublayer.getComputingSize();
            }
            else if (dnn_sublayer.isLinkDNNSublayer()) {
                this->layered_graph[link] += dnn_sublayer.getOutput().getLinkSize();
            }
        }
    }


    void LayeredGraph::recordVirtualQueue() {
        if (this->layer_node_virtual_queue_recorder_table.size() == 0){
            throw runtime_error("Please initialize virtual_queue_recorder_table first.");
        }
        else {
            map<string, double> node_virtual_backlogs;

            for (const auto &pair : this->layered_graph) {
                LayerNodePair virtual_link = pair.first;
                double layer_node_virtual_backlog = pair.second;

                cOutVector *layer_node_virtual_queue_recorder = this->layer_node_virtual_queue_recorder_table[virtual_link];
                layer_node_virtual_queue_recorder->record(layer_node_virtual_backlog);

                node_virtual_backlogs[virtual_link.getSrcLayerNode().toNodeName() + "->" + virtual_link.getDstLayerNode().toNodeName()] += layer_node_virtual_backlog;
            }

            for (const auto &pair : node_virtual_backlogs) {
                string node_virtual_link = pair.first;
                double node_virtual_backlog = node_virtual_backlogs.at(node_virtual_link);

                cOutVector *node_virtual_queue_recorder = this->node_virtual_queue_recorder_table[node_virtual_link];
                node_virtual_queue_recorder->record(node_virtual_backlog);
            }
        }
    }
}
