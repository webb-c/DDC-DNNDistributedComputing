/*
 * VirtualBacklog.h
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#ifndef __DDC_VIRTUALBACKLOG_H_
#define __DDC_VIRTUALBACKLOG_H_

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"
#include "ddc/dnn/DNNSublayer.h"

using namespace std;

namespace inet{
    class VirtualBacklog {
        private:
            map<LayerNodePair, vector<DNNSublayer>> virtual_backlogs;

        public:
            VirtualBacklog();

            vector<DNNSublayer> &returnVirtualBacklog(LayerNodePair layer_node_pair);

            void addVirtualBacklog(LayerNodePair layer_node_pair);
            bool isVirtualBacklogExistByLayerNodePair(LayerNodePair layer_node_pair);
            void resetVirtualBacklog();

            bool pushDNNSublayerToVirtualBacklogByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair);
            DNNSublayer popDNNSublayerToVirtualBacklogByLayerNodePair(DNNOutput dnn_output, LayerNodePair layer_node_pair);

            bool isVirtualQueueEmpty(LayerNodePair layer_node_pair);
            double getBacklogLengthByLayerNodePair(LayerNodePair layer_node_pair);
            double getComputingBacklogLength(const vector<DNNSublayer>& virtual_backlog);
            double getLinkBacklogLength(const vector<DNNSublayer>& virtual_backlog);
    };
}

#endif /* __DDC_VIRTUALBACKLOG_H_ */
