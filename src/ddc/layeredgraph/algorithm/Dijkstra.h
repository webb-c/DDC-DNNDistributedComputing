/*
 * Dijkstra.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DDC_DIJKSTRA_H
#define __DDC_DIJKSTRA_H

#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <map>
#include <set>
#include <algorithm>

#include "ddc/layeredgraph/algorithm/AlgorithmBase.h"

using namespace std;
using namespace omnetpp;

namespace inet {

class Dijkstra : public AlgorithmBase{
  private:
    void initializeDistances(const vector<LayerNode>& layer_nodes,
                             map<LayerNode, double>& distances,
                             const LayerNode& src_layer_node);

    double getEdgeWeight(const map<LayerNodePair, double>& layered_graph,
                         const LayerNode& src,
                         const LayerNode& dst);

    vector<LayerNode> reconstructPath(const map<LayerNode, LayerNode>& previous,
                                           const LayerNode& src_layer_node,
                                           const LayerNode& dst_layer_node);
  public:
    Dijkstra();

    vector<LayerNode> findPath(const map<LayerNodePair, double>& layered_graph,
                                    const map<LayerNode, std::vector<LayerNode>>& adjacency_matrix,
                                    const vector<LayerNode> layer_nodes,
                                    const LayerNode& src_layer_node,
                                    const LayerNode& dst_layer_node) override;


};

}

#endif // ifndef __DDC_DIJKSTRA_H

