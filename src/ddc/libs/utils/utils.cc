/*
 * utils.cc
 *
 *  Created on: 2024. 01. 02.
 *      Author: Suhwan
 */

#include "ddc/libs/utils/utils.h"

using namespace std;
using namespace omnetpp;

namespace inet {

int returnNodeNumberFromNode(cModule *node){
    string node_name = node->getFullName();
    if (node_name.length() < 4) {
        throw node_name + " is shorter than 4.";
    }
    int node_num = std::stoi(node_name.substr(4));

    return node_num;
}

int returnNodeNumberFromNodeName(string node_name){
    int node_num = std::stoi(node_name.substr(4));

    return node_num;
}

void addDummyPayloadToPacket(Packet *packet, int packet_size) {
    auto dummy_payload = makeShared<ByteCountChunk>(B(packet_size));
    packet->insertAtBack(dummy_payload);
}


}
