/*
 * ProcessingUnit.cc
 *
 *  Created on: 2024. 01. 01.
 *      Author: Suhwan
 */

#include "ddc/processingunit/ProcessingUnit.h"

namespace inet {

Define_Module(ProcessingUnit);

ProcessingUnit::ProcessingUnit() {
}

ProcessingUnit::~ProcessingUnit() {}

void ProcessingUnit::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        this->compute_capacity = par("compute_capacity");
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        init();
    }
}

void ProcessingUnit::init() {
    findAndsetComputeDNNApplication();
}

void ProcessingUnit::handleMessageWhenUp(cMessage *msg) {
    if (strcmp(msg->getFullName(), "compute") == 0){
        handleComputeMessage(msg);
    }
    else if (strcmp(msg->getFullName(), "can_compute") == 0){
        handleCanComputeMessage(msg);
    }
    else if (strcmp(msg->getFullName(), "reset") == 0){
        handleResetMessage();
        delete msg;
    }
}

void ProcessingUnit::findAndsetComputeDNNApplication() {
    this->compute_dnn_application = getParentModule()->getSubmodule("app", 0);
    this->local_port = par("local_port").intValue();
}

void ProcessingUnit::registercMessage(cMessage *msg) {
    this->msg = msg;
}

void ProcessingUnit::loadComputeAndSublayer() {
    loadCompute();
    loadSublayer();
}

void ProcessingUnit::loadCompute(){
    this->compute_packet = dynamic_cast<Compute *>(this->msg);
}

void ProcessingUnit::loadSublayer() {
    this->dnn_sublayer = this->compute_packet->getDnn_sublayer();
}

void ProcessingUnit::computeDNNSublayerAndsendDNNOutput(){
    computeDNNSublayer();

    ComputeFinish *compute_finish_message = new ComputeFinish("compute_finish");

    DNNOutput *dnn_output = this->dnn_output.deepcopy();
    compute_finish_message->setDnn_output(dnn_output);

    sendDirect(compute_finish_message, 0, this->compute_time, this->compute_dnn_application, "dataIn");
    scheduleAt(simTime() + this->compute_time, this->can_compute_message);
}

void ProcessingUnit::computeDNNSublayer() {
    this->dnn_output = this->dnn_sublayer->getOutput();

    double computing_size = this->dnn_sublayer->getComputingSize();

    // Calculate compute time simply dividing.
    this->compute_time = computing_size / this->compute_capacity;
}

void ProcessingUnit::handleComputeMessage(cMessage *msg){
    if (this->isBusy()) {
        enqueueMessage(msg);
        return;
    }

    registercMessage(msg);
    loadComputeAndSublayer();
    computeDNNSublayerAndsendDNNOutput();

    delete msg;
}

void ProcessingUnit::handleCanComputeMessage(cMessage *msg){
    if (!this->message_queue.empty()) {
        cMessage *msg = dequeueMessage();
        scheduleAt(simTime(), msg);
    }
}

void ProcessingUnit::handleResetMessage() {
    reset();
}

void ProcessingUnit::reset() {
    if (isBusy()) {
        cancelEvent(this->can_compute_message);
    }

    while (!this->message_queue.empty()) {
        cMessage *msg = dequeueMessage();
        delete msg;
    }

    this->computing_queue_length = 0;
}

void ProcessingUnit::enqueueMessage(cMessage *msg){
    Compute *compute_packet = dynamic_cast<Compute *>(msg);
    double computing_size = compute_packet->getDnn_sublayer()->getComputingSize();
    this->computing_queue_length += computing_size;

    this->message_queue.push(msg);
}

cMessage *ProcessingUnit::dequeueMessage(){
    cMessage *msg = this->message_queue.front();
    this->message_queue.pop();

    Compute *compute_packet = dynamic_cast<Compute *>(msg);
    double computing_size = compute_packet->getDnn_sublayer()->getComputingSize();
    this->computing_queue_length -= computing_size;

    return msg;
}

bool ProcessingUnit::isBusy(){
    return this->can_compute_message->isScheduled();
}

double ProcessingUnit::getComputingQueueLength() {
    return this->computing_queue_length;
}

void ProcessingUnit::socketDataArrived(UdpSocket *socket, Packet *packet) {}
void ProcessingUnit::socketErrorArrived(UdpSocket *socket, Indication *indication) {}
void ProcessingUnit::socketClosed(UdpSocket *socket) {}

void ProcessingUnit::handleStartOperation(LifecycleOperation *operation) {}
void ProcessingUnit::handleStopOperation(LifecycleOperation *operation) {}
void ProcessingUnit::handleCrashOperation(LifecycleOperation *operation) {}

} //namespace inet
