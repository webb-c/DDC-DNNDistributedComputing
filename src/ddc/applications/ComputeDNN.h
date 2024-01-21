/*
 * ComputeDNN.h
 *
 *  Created on: 2023. 12. 22.
 *      Author: Suhwan
 */

#ifndef __DDC_COMPUTEDNN_H
#define __DDC_COMPUTEDNN_H

#include <iostream>
#include <omnetpp.h>

#include "ddc/libs/utils/utils.h"

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "inet/common/socket/SocketMap.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/packet/chunk/Chunk.h"

#include "ddc/processingunit/ProcessingUnit.h"

#include "ddc/applications/NodeInformation.h"

#include "ddc/dnn/DNNSublayer.h"
#include "ddc/dnn/DNNOutput.h"
#include "ddc/dnn/DNNSublayerConfig.h"
#include "ddc/backlog/PhysicalBacklog.h"
#include "ddc/backlog/VirtualBacklog.h"
#include "ddc/dnn/DNNSublayerFactory.h"
#include "ddc/dnn/DNNSublayerTable.h"

#include "ddc/message/Compute_m.h"
#include "ddc/message/ComputeFinish_m.h"
#include "ddc/message/Schedule_m.h"
#include "ddc/message/Route_m.h"
#include "ddc/message/ResponseNodeInformation_m.h"

using namespace std;
using namespace omnetpp;

namespace inet {

class ComputeDNN : public ApplicationBase, public TcpSocket::ICallback
{
  private:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    bool is_reset = false;

    string START_DELIMITER = "<send>";
    string END_DELIMITER = ">send<";

    map<LayerNodePair, cOutVector *> layer_node_virtual_queue_recorder_table;
    map<string, cOutVector *> node_virtual_queue_recorder_table;
    cOutVector *end_to_end_latency_recorder;
    UdpSocket socket;
    TcpSocket tcp_socket;
    SocketMap tcp_in_socket_map;
    map<int, string> socket_data_queue;
    map<string, TcpSocket *> tcp_out_socket_map;
    int local_port; // Local port
    int dest_port;  // Destination port
    int controlplane_port;
    std::vector<L3Address> dest_addresses; // Destination address
    vector<string> dest_addresses_str;
    double virtual_queue_record_duration;

    ProcessingUnit *processing_unit;

    int layer_depth;
    int node_index;
    string node_name;
    vector<LayerNode> layer_node_vector;
    vector<LayerNodePair> virtual_links;

    PhysicalBacklog physical_backlog;
    VirtualBacklog virtual_backlog;
    DNNSublayerFactory dnn_sublayer_factory;
    DNNSublayerTable dnn_sublayer_table;
    vector<ComputeFinish *> dnn_output_vector;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    void init();
    void parseParameters();
    void parseAddress();
    void initLayerNodeOfMe();
    void initLayerNodePair();
    void initVirtualQueue();
    void initRecorder();
    void initProcessingUnit();
    void initTcpSockets();
    L3Address chooseDestAddr(int next_node_index);
    L3Address chooseDestAddr(string dst_name);

    virtual void handleMessageWhenUp(cMessage *msg) override;

    void handleScheduleMessage(Schedule *msg);
    void handleComputeFinishMessage(ComputeFinish *msg);
    void handleResetMessage();
    void handleStartMessage();
    void handleRequestInformationMessage();

    Packet *returnResponseNodeInformationMessage(NodeInformation &node_information);

    ProcessingUnit* findProcessingUnit();
    void sendComputeMessage(DNNSublayer sublayer);
    void sendDNNSublayerToNextNode(DNNSublayer dnn_sublayer);
    void sendDNNSublayerToNextLayerNode(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair);

    DNNSublayerConfig *peekDNNSublayerConfigFromSchedule(Schedule *msg);
    const DNNOutput *peekDNNOutputFromComputeFinish(ComputeFinish *msg);

    void reset();
    void start();

    void recordVirtualQueue();
    void recordEndToEndLatency(double latency);

    virtual void socketDataArrived(TcpSocket *socket, Packet *packet, bool urgent) override;
    virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override;
    virtual void socketEstablished(TcpSocket *socket) override {}
    virtual void socketPeerClosed(TcpSocket *socket) override {}
    virtual void socketClosed(TcpSocket *socket) override {}
    virtual void socketFailure(TcpSocket *socket, int code) override {}
    virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override {}
    virtual void socketDeleted(TcpSocket *socket) override { tcp_in_socket_map.removeSocket(socket); }

    void handleStartOperation(LifecycleOperation *operation) override;
    void handleStopOperation(LifecycleOperation *operation) override;
    void handleCrashOperation(LifecycleOperation *operation) override;

  public:
    ComputeDNN();
    ~ComputeDNN();

    vector<string> getDestAddressStr() {
        return this->dest_addresses_str;
    }

    double getQueueLengthByLayerNodePair(LayerNodePair link);

    string getNodeName() { return this->node_name; }
    vector<LayerNodePair> getLinks() { return this->virtual_links; }

    VirtualBacklog getVirtualBacklog() { return this->virtual_backlog; }
};

} //namespace inet

#endif // ifndef __DDC_COMPUTEDNN_H
