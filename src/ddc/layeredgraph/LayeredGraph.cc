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
        resetNodeUpdatedIndicator();
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

    void LayeredGraph::initNodeUpdatedIndicator() {
        resetNodeUpdatedIndicator();
    }

    void LayeredGraph::resetNodeUpdatedIndicator() {
        for (int i = 1; i < this->node_num + 1; i++) {
            string node_name = "node" + to_string(i);
            this->node_updated_indicator[node_name] = false;
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

    void LayeredGraph::update(NodeInformation node_information){
        string node_name = node_information.getNodeName();
        map<LayerNodePair, double> links_information = node_information.getLinksInformation();

        if (links_information.size() == 0) { throw runtime_error(node_information.serialize()); }

        for (auto &e : links_information) {
            this->layered_graph[e.first] = links_information[e.first];
        }

        this->node_updated_indicator[node_name] = true;
    }

    bool LayeredGraph::needUpdate() {
        for (const auto &e : this->node_updated_indicator) {
            if (this->node_updated_indicator[e.first] == false) {
                return true;
            }
        }
        return false;
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
}
