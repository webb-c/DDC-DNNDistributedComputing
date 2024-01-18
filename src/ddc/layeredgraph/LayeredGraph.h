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
    map<string, bool> node_updated_indicator;

    AlgorithmBase *algorithm;

    // nodes
    void addNode();

    // nodes_application
    void addComputeDNNApplication();

    // layer_nodes
    void addLayerNodesFromNode();
    void addLayerNode(LayerNode layer_node);

    // layer_node_node_table
    void addLayerNodeNodeTable();

    // lyaer_node_application_table
    void addLayerNodeApplicationTable();

    void initNodeUpdatedIndicator();

  public:
    LayeredGraph();
    LayeredGraph(int graph_depth, int node_num, AlgorithmBase *algorithm);

    ComputeDNN *returnComputeDNNApplication(cModule *node);

    void registerNode(cModule *node);

    void buildAdjacencyMatrix();
    void update(NodeInformation node_information);
    void initLayeredGraph();

    void resetNodeUpdatedIndicator();
    bool needUpdate();

    vector<LayerNode> findPath(LayerNode src_layer_node, LayerNode dst_layer_node);
    double getArrivalRate(LayerNode src_layer_node, LayerNode dst_layer_node);

    void applyVirtualUpdate(vector<DNNSublayer> &dnn_sublayer_vector);
};

}

#endif // ifndef __DDC_LAYEREDGRAPH_H
