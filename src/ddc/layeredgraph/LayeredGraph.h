/*
 * LayeredGraph.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DDC_LAYEREDGRAPH_H
#define __DDC_LAYEREDGRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <omnetpp.h>

#include "ddc/libs/utils/utils.h"

#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"
#include "ddc/layernode/LayerNodePairPath.h"

#include "ddc/applications/ComputeDNN.h"
#include "ddc/applications/NodeInformation.h"

#include "ddc/layeredgraph/algorithm/AlgorithmBase.h"
#include "ddc/layeredgraph/algorithm/Dijkstra.h"

using namespace std;
using namespace omnetpp;

namespace inet {

class LayeredGraph{
  private:
    cModule *node;
    int graph_depth;
    int node_num;
    vector<cModule *> nodes;
    vector<ComputeDNN *> nodes_application;
    vector<LayerNode> layer_nodes;
    map<LayerNode, cModule *> layer_node_node_table;
    map<LayerNode, ComputeDNN *> layer_node_application_table;
    map<LayerNode, vector<LayerNode>> adjacency_matrix;
    map<LayerNodePair, double> layered_graph;
    map<int, double> node_link_capacity_table;
    map<int, double> node_compute_capacity_table;
    map<string, bool> node_sync_indicator;

    AlgorithmBase *algorithm;

    vector<int> link_job_nums;
    vector<int> compute_job_nums;

    double latest_update_time = 0.0;

    map<LayerNodePair, cOutVector *> layer_node_virtual_queue_recorder_table;
    map<string, cOutVector *> node_virtual_queue_recorder_table;

    // nodes
    void addNode();

    // nodes_application
    void addComputeDNNApplication();

    // layer_nodes
    void addLayerNodesFromNode();
    void addLayerNode(LayerNode layer_node);

    // layer_node_node_table
    void addLayerNodeNodeTable();

    // layer_node_application_table
    void addLayerNodeApplicationTable();

    // node_link_capacity_table, node_compute_capacity_table
    void addNodeCapacityTable();

    void initNodeSyncIndicator();

    void updateLink(vector<int> &link_job_nums);
    void updateCompute(vector<int> &compute_job_nums);

  public:
    LayeredGraph();
    LayeredGraph(int graph_depth, int node_num, AlgorithmBase *algorithm);

    ComputeDNN *returnComputeDNNApplication(cModule *node);

    void registerNode(cModule *node);

    void buildAdjacencyMatrix();
    void sync(NodeInformation node_information);
    void update();
    void initLayeredGraph();

    void resetNodeSyncIndicator();

    vector<LayerNode> findPath(LayerNode src_layer_node, LayerNode dst_layer_node);
    double getArrivalRate(LayerNode src_layer_node, LayerNode dst_layer_node);

    void applyVirtualUpdate(vector<DNNSublayer> &dnn_sublayer_vector);

    void initRecorder();
    void recordVirtualQueue();
};

}

#endif // ifndef __DDC_LAYEREDGRAPH_H
