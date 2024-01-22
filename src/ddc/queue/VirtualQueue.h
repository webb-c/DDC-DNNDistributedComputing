/*
 * VirtualQueue.h
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
    class VirtualQueue {
        private:
            map<LayerNodePair, vector<DNNSublayer>> virtual_queues;

        public:
            VirtualQueue();

            vector<DNNSublayer> &returnVirtualQueue(LayerNodePair layer_node_pair);

            void addVirtualQueue(LayerNodePair layer_node_pair);
            bool isVirtualQueueExistByLayerNodePair(LayerNodePair layer_node_pair);
            void resetVirtualQueue();

            bool pushDNNSublayerToVirtualQueueByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair);
            DNNSublayer popDNNSublayerToVirtualQueueByLayerNodePair(DNNOutput dnn_output, LayerNodePair layer_node_pair);

            bool isVirtualQueueEmpty(LayerNodePair layer_node_pair);
            double getQueueLengthByLayerNodePair(LayerNodePair layer_node_pair);
            double getComputingQueueLength(const vector<DNNSublayer>& virtual_queue);
            double getLinkQueueLength(const vector<DNNSublayer>& virtual_queue);
    };
}

#endif /* __DDC_VIRTUALBACKLOG_H_ */
