/*
 * AlgorithmBase.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DDC_ALGORITHMBASE_H
#define __DDC_ALGORITHMBASE_H

#include <iostream>

#include "../../layernode/LayerNodePairPath.h"
#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"

using namespace std;
using namespace omnetpp;

namespace inet {

class AlgorithmBase{
  public:
    virtual vector<LayerNode> findPath(const map<LayerNodePair, double>& layered_graph,
                                    const map<LayerNode, vector<LayerNode>>& adjacency_matrix,
                                    const vector<LayerNode> layer_nodes,
                                    const LayerNode& src_layer_node,
                                    const LayerNode& dst_layer_node) = 0;
};

}

#endif // ifndef __DDC_ALGORITHMBASE_H
