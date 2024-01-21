/*
 * Controlplane.h
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#ifndef __DDC_CONTROLPLANE_H_
#define __DDC_CONTROLPLANE_H_

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

#include "ddc/applications/NodeInformation.h"

#include "ddc/dnn/DNNSublayer.h"
#include "ddc/dnn/DNNOutput.h"
#include "ddc/dnn/DNNSublayerConfig.h"
#include "ddc/dnn/DNNSublayerFactory.h"
#include "ddc/dnn/DNNSublayerTable.h"
#include "ddc/dnn/DNNInformation.h"
#include "ddc/dnn/DNNSublayerConfigFactory.h"
#include "ddc/queue/VirtualQueue.h"
#include "ddc/layernode/LayerNodePairPath.h"
#include "ddc/layeredgraph/LayeredGraph.h"
#include "ddc/layeredgraph/algorithm/Dijkstra.h"

#include "ddc/message/Compute_m.h"
#include "ddc/message/ComputeFinish_m.h"
#include "ddc/message/Schedule_m.h"
#include "ddc/message/Route_m.h"
#include "ddc/message/Reset_m.h"
#include "ddc/message/RequestArrivalRate_m.h"
#include "ddc/message/ResponseArrivalRate_m.h"
#include "ddc/message/RequestNodeInformation_m.h"
#include "ddc/message/ResponseNodeInformation_m.h"



using namespace std;
using namespace omnetpp;

namespace inet {

class Controlplane : public ApplicationBase, public UdpSocket::ICallback
{
  private:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    bool is_reset = false;

    UdpSocket socket;
    int local_port = -1; // Local port
    int dest_port = -1;  // Destination port
    vector<L3Address> dest_addresses; // Destination address
    vector<string> dest_addresses_str;
    vector<cModule *> dest_nodes;
    string routing_algorithm;

    DNNSublayerConfigFactory dnn_sublayer_config_factory;
    DNNSublayerFactory dnn_sublayer_factory;

    int layer_depth;
    int node_num;
    vector<LayerNode> layer_node_vector;
    LayeredGraph layered_graph;
    bool requested_update = false;
    vector<cMessage *> job_queue;

    cMessage *request_information_time_out = new cMessage("request_information_time_out");

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    void init();
    void parseParameters();
    void parseAddress();
    L3Address chooseDestAddr(int next_node_index);
    L3Address chooseDestAddr(string name);

    void initDataplane();
    void loadAndinitLayeredGraph();
    void initLayeredGraph();
    void loadLayeredGraph();
    void updateLayeredGraphVirtually(vector<DNNSublayerConfig *> dnn_sublayer_config_vector);

    void handleMessageWhenUp(cMessage *msg) override;

    void handleJobMessage(cMessage *msg);
    void handleRouteMessage(Route *msg);
    void handleArrivalRateMessage(RequestArrivalRate *request_arrival_rate);
    void doAllJobs();
    void handleNodeInformationMessage(Packet *response_node_information);
    void handleResetMessage();
    void handleStartMessage();

    DNNInformation *peekDNNInformationFromRoute(Route *msg);
    NodeInformation peekNodeInformationFromRoute(Packet *response_node_information);
    Schedule *returnScheduleMessage(DNNSublayerConfig *dnn_sublayer_config);

    void reset();
    void start();
    void sendResetMessageToNodes();
    void sendStartMessageToNodes();
    void sendReqeustInformationMessageToNodes();

    void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    void socketClosed(UdpSocket *socket) override;

    void handleStartOperation(LifecycleOperation *operation) override;
    void handleStopOperation(LifecycleOperation *operation) override;
    void handleCrashOperation(LifecycleOperation *operation) override;

  public:
    Controlplane();
    ~Controlplane();
};

}; // namespace

#endif /* __DDC_CONTROLPLANE_H_ */
