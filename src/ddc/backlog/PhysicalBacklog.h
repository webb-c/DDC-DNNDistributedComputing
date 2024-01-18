/*
 * PhysicalBacklog.h
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#ifndef __DDC_PHYSICALBACKLOG_H_
#define __DDC_PHYSICALBACKLOG_H_

#include <iostream>
#include <map>
#include <queue>

#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"
#include "ddc/dnn/DNNSublayer.h"

using namespace std;

namespace inet{
    class PhysicalBacklog {
        private:
        map<LayerNodePair, queue<DNNSublayer>> physical_backlogs;

        queue<DNNSublayer> &returnPhysicalBacklog(LayerNodePair layer_node_pair);

        public:
            PhysicalBacklog();

            void addPhysicalBacklog(LayerNodePair layer_node_pair);

            bool isPhysicalBacklogExistByLayerNodePair(LayerNodePair layer_node_pair);
            queue<DNNSublayer> &getPhysicalBacklogByLayerNodePair(LayerNodePair layer_node_pair);
            bool pushDNNSublayerToPhysicalBacklogByLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair);
            bool isDNNSublayerExistByDNNSublayerLayerNodePair(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair);
            DNNSublayer popDNNSublayerToPhysicalBacklogByLayerNodePair(LayerNodePair layer_node_pair);

            bool isPhysicalBacklogEmpty(LayerNodePair layer_node_pair);
            double getBacklogLengthByLayerNodePair(LayerNodePair layer_node_pair);
            double getComputingBacklogLength(queue<DNNSublayer> &physical_backlog);
            double getLinkBacklogLength(queue<DNNSublayer> &physical_backlog);

    };
}


#endif /* __DDC_PHYSICALBACKLOG_H_ */
