/*
 * Agent.h
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#ifndef __DDC_AGENT_H_
#define __DDC_AGENT_H_

#include <iostream>
#include <omnetpp.h>

#include "ddc/libs/utils/utils.h"

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/packet/chunk/Chunk.h"
#include <inet/common/packet/chunk/BytesChunk.h>

#include "ddc/dnn/DNNSublayer.h"
#include "ddc/dnn/DNNOutput.h"
#include "ddc/dnn/DNNConfigFactory.h"
#include "ddc/dnn/DNNSublayerConfig.h"
#include "ddc/dnn/DNNSublayerFactory.h"
#include "ddc/dnn/DNNSublayerTable.h"
#include "ddc/dnn/DNNInformation.h"
#include "ddc/dnn/DNNSublayerConfigFactory.h"
#include "ddc/communicator/Communicator.h"
#include "ddc/layernode/LayerNode.h"

#include "ddc/message/Route_m.h"
#include "ddc/message/RequestArrivalRate_m.h"
#include "ddc/message/ResponseArrivalRate_m.h"

using namespace std;
using namespace omnetpp;

namespace inet {

string ACTION("action");
string REWARD("reward");
string FINISH("finish");
string ACK("ACK");

class Agent : public ApplicationBase, public UdpSocket::ICallback
{
  private:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    UdpSocket socket;
    int local_port = -1; // Local port
    int dest_port = -1;  // Destination port
    vector<L3Address> dest_addresses; // Destination address
    vector<string> dest_addresses_str;
    vector<cModule *> dest_nodes;

    DNNConfigFactory dnn_config_factory;

    string pipe_name;
    Communicator communicator;
    double skip_length;

    int layer_depth;
    string src_node_name;
    string dst_node_name;
    LayerNode src_layer_node;
    LayerNode dst_layer_node;
    int src_layer_node_index;
    int dst_layer_node_index;
    string model_name;
    string agent_name;
    string node_name;

    int dnn_identifier;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    void init();
    void parseParameters();
    void parseAddress();
    L3Address chooseDestAddr(string dst_name);
    void initCommunicator();
    void initSrcDstLayerNode();
    void initNodeName();
    void initVariable();

    void handleMessageWhenUp(cMessage *msg) override;

    Route *returnRouteMessage(DNNInformation *dnn_information);
    RequestArrivalRate *returnRequestArrivalRateMesseage();

    void sendMessageToControlPlane(Route *route, int index = -1);
    void sendMessageToControlPlane(RequestArrivalRate *request_arrival_rate, int index = -1);
    void sendMessageToControlPlane(Packet *packet, int index = -1);

    void handleAction();
    void handleRoute();
    void handleReward();
    void handleFinish();
    void handleReset();
    void handleStart();
    void handleResponseArrivalRate(ResponseArrivalRate *response_arrival_rate);

    void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    void socketClosed(UdpSocket *socket) override;

    void handleStartOperation(LifecycleOperation *operation) override;
    void handleStopOperation(LifecycleOperation *operation) override;
    void handleCrashOperation(LifecycleOperation *operation) override;

  public:
    Agent();
    ~Agent();
};

}; // namespace

#endif /* __DDC_AGENT_H_ */
