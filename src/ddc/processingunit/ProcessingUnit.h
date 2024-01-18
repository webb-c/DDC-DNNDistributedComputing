/*
 * ProcessingUnit.h
 *
 *  Created on: 2024. 01. 01.
 *      Author: Suhwan
 */

#ifndef __DDC_PROCESSINGUNIT_H
#define __DDC_PROCESSINGUNIT_H

#include <iostream>
#include <queue>
#include <omnetpp.h>

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/packet/chunk/Chunk.h"

#include "ddc/dnn/DNNSublayer.h"
#include "ddc/dnn/DNNOutput.h"

#include "ddc/message/Compute_m.h"
#include "ddc/message/ComputeFinish_m.h"

using namespace std;
using namespace omnetpp;

namespace inet {

class ProcessingUnit : public ApplicationBase, public UdpSocket::ICallback
{
  private:
    enum SelfMsgKinds { START = 1, SEND, STOP };

    UdpSocket socket;
    int local_port = -1; // Local port
    double compute_capacity;

    cMessage *can_compute_message = new cMessage("can_compute");
    bool busy = false;

    cModule* compute_dnn_application;
    cMessage *msg;
    Compute *compute_packet;
    const DNNSublayer *dnn_sublayer;
    DNNOutput dnn_output;
    double compute_time;
    queue<cMessage *> message_queue;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    void init();
    virtual void handleMessageWhenUp(cMessage *msg) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    void socketClosed(UdpSocket *socket) override;

    void handleStartOperation(LifecycleOperation *operation) override;
    void handleStopOperation(LifecycleOperation *operation) override;
    void handleCrashOperation(LifecycleOperation *operation) override;

    void findAndsetComputeDNNApplication();

    void registercMessage(cMessage *msg);
    void loadComputeAndSublayer();
    void loadCompute();
    void loadSublayer();
    void computeDNNSublayerAndsendDNNOutput();
    void computeDNNSublayer();

    void enqueueMessage(cMessage *msg);
    cMessage *dequeueMessage();

    void reset();

    void handleComputeMessage(cMessage *msg);
    void handleCanComputeMessage(cMessage *msg);
    void handleResetMessage();

  public:
    ProcessingUnit();
    virtual ~ProcessingUnit();

    bool isBusy();
};

} //namespace inet

#endif // ifndef __DDC_PROCESSINGUNIT_H
