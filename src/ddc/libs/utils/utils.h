/*
 * utils.h
 *
 *  Created on: 2024. 01. 02.
 *      Author: Suhwan
 */

#ifndef __DDC_UTILS_H
#define __DDC_UTILS_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <omnetpp.h>

#include "inet/common/packet/Packet.h"

#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"

#include "ddc/applications/ComputeDNN.h"

#include "ddc/layeredgraph/algorithm/AlgorithmBase.h"
#include "ddc/layeredgraph/algorithm/Dijkstra.h"

using namespace std;
using namespace omnetpp;

namespace inet {

int returnNodeNumberFromNode(cModule *node);
int returnNodeNumberFromNodeName(string node_name);
void addDummyPayloadToPacket(Packet *packet, int packet_size);

}

#endif // ifndef __DDC_UTILS_H
