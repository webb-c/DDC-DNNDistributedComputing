/*
 * Agent.cc
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#include "Agent.h"

namespace inet {

Define_Module(Agent);

Agent::Agent() {}

Agent::~Agent() {}

void Agent::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        parseParameters();
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        init();
    }
}

void Agent::init() {
    socket.setOutputGate(gate("socketOut"));
    socket.bind(L3Address(), this->local_port);

    parseAddress();
    initCommunicator();
    initSrcDstLayerNode();
    initNodeName();
    initVariable();
}

void Agent::parseAddress(){
    const char *dest_addresses = par("dest_addresses");
    cStringTokenizer tokenizer(dest_addresses);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
        this->dest_addresses.push_back(result);
        this->dest_addresses_str.push_back(token);
    }
}

L3Address Agent::chooseDestAddr(string dst_name) {
    L3Address result;
    L3AddressResolver().tryResolve(dst_name.c_str(), result);
    if (result.isUnspecified())
        EV_ERROR << "cannot resolve destination address: " << dst_name << endl;

    return result;
}


void Agent::parseParameters() {
    this->layer_depth = par("layer_depth").intValue();
    this->local_port = par("local_port").intValue();
    this->dest_port = par("dest_port").intValue();
    this->pipe_name = par("pipe_name").stdstringValue();
    this->skip_length = par("skip_length").doubleValue();
    this->src_node_name = par("src_node_name").stdstringValue();
    this->dst_node_name = par("dst_node_name").stdstringValue();
    this->model_name = par("model_name").stdstringValue();
    this->agent_name = par("agent_name").stdstringValue();
}

void Agent::initCommunicator() {
    this->communicator = Communicator(this->pipe_name);
}

void Agent::initSrcDstLayerNode() {
    DNNConfig dnn_config = dnn_config_factory.makeDNNConfig(this->model_name);
    int sublayer_num = dnn_config.getSublayerNum();

    this->src_layer_node_index = returnNodeNumberFromNodeName(this->src_node_name);
    this->dst_layer_node_index = returnNodeNumberFromNodeName(this->dst_node_name);

    this->src_layer_node = LayerNode(this->src_layer_node_index, 1);
    this->dst_layer_node = LayerNode(this->dst_layer_node_index, sublayer_num - 1);
}

void Agent::initNodeName() {
    this->node_name = getParentModule()->getFullName();
}

void Agent::initVariable() {
    this->dnn_identifier = 0;
}

void Agent::handleMessageWhenUp(cMessage *msg) {
    if (strcmp(msg->getName(), "wait_for_RL") == 0){

        this->communicator.sendPythonMessage("waiting");
        string python_msg = this->communicator.getPythonMessage();

        if(python_msg.compare(ACTION) == 0) {
            handleAction();
        }
        else if (python_msg.compare(REWARD) == 0) {
            handleReward();
        }
        else if (python_msg.compare(FINISH) == 0) {
            handleFinish();
        }
    }
    else if (strcmp(msg->getName(), "reset") == 0) {
        handleReset();
    }
    else if (strcmp(msg->getName(), "start") == 0) {
        handleStart();
    }
    else if (strcmp(msg->getName(), "response_arrival_rate") == 0) {
        ResponseArrivalRate *response_arrival_rate = dynamic_cast<ResponseArrivalRate *>(msg);
        handleResponseArrivalRate(response_arrival_rate);
    }
    else if (msg->isSelfMessage() && strcmp(msg->getName(), "route") == 0) {
        handleRoute();
    }

    delete msg;
}

Route *Agent::returnRouteMessage(DNNInformation *dnn_information) {
    Route *route = new Route("route");

    string dnn_information_config_serialized = dnn_information->serialize();

    vector<uint8_t> dnn_information_config_serialized_vector(dnn_information_config_serialized.begin(), dnn_information_config_serialized.end());
    const auto& dnn_information_config_chunk = makeShared<BytesChunk>(dnn_information_config_serialized_vector);

    route->insertAtBack(dnn_information_config_chunk);

    return route;
}

void Agent::handleAction() {
    this->communicator.sendPythonMessage("ACK");
    string action = this->communicator.getPythonMessage();

    EV << "action is " << action << endl;

    double skip_time = stof(action);

    scheduleAt(simTime() + skip_time, new cMessage("route"));
}

void Agent::handleRoute() {
    double start_simulation_time = simTime().dbl();

    DNNID dnn_id = DNNID(this->model_name, this->agent_name, start_simulation_time, this->dnn_identifier++);
    DNNInformation *dnn_information = new DNNInformation(dnn_id, this->src_layer_node, this->dst_layer_node);

    Route *route = returnRouteMessage(dnn_information);
    sendMessageToControlPlane(route);

    scheduleAt(simTime(), new cMessage("wait_for_RL"));
}

void Agent::handleReward() {
    RequestArrivalRate *request_arrival_rate = returnRequestArrivalRateMesseage();

    addDummyPayloadToPacket(request_arrival_rate, 1);

    sendMessageToControlPlane(request_arrival_rate);
}

RequestArrivalRate *Agent::returnRequestArrivalRateMesseage() {
    RequestArrivalRate *request_arrival_rate = new RequestArrivalRate("request_arrival_rate");
    request_arrival_rate->setAgent_name(this->agent_name.c_str());
    request_arrival_rate->setSrc_layer_node(this->src_layer_node);
    request_arrival_rate->setDst_layer_node(this->dst_layer_node);
    request_arrival_rate->setCallback_node_name(this->node_name.c_str());
    request_arrival_rate->setCallback_port(this->local_port);

    return request_arrival_rate;
}

void Agent::handleResponseArrivalRate(ResponseArrivalRate *response_arrival_rate) {
    double arrival_rate = response_arrival_rate->getArrival_rate();
    this->communicator.sendPythonMessage(to_string(arrival_rate));

    scheduleAt(simTime(), new cMessage("wait_for_RL"));

    string return_message = this->communicator.getPythonMessage();      // ACK
    if(return_message.compare(ACK) != 0){
        EV << "error in reward " << return_message << endl;
        return;
    }
}

void Agent::handleFinish() {
    scheduleAt(simTime(), new cMessage("reset"));
    scheduleAt(simTime() + this->skip_length - 10, new cMessage("start"));
}

void Agent::handleReset() {
    // reset
    Packet* reset_message = new Packet("reset");
    addDummyPayloadToPacket(reset_message, 1);

    sendMessageToControlPlane(reset_message);

    scheduleAt(simTime() + this->skip_length, new cMessage("wait_for_RL"));
}

void Agent::handleStart() {
    // reset
    Packet* start_message = new Packet("start");
    addDummyPayloadToPacket(start_message, 1);

    sendMessageToControlPlane(start_message);
}

void Agent::sendMessageToControlPlane(Route *route, int index) {
    if (index == -1) {
        L3Address dest_address = chooseDestAddr("controlplane");
        socket.sendTo(route, dest_address, this->dest_port);
    }
}

void Agent::sendMessageToControlPlane(RequestArrivalRate *request_arrival_rate, int index) {
    if (index == -1) {
        L3Address dest_address = chooseDestAddr("controlplane");
        socket.sendTo(request_arrival_rate, dest_address, this->dest_port);
    }
}

void Agent::sendMessageToControlPlane(Packet *packet, int index) {
    if (index == -1) {
        L3Address dest_address = chooseDestAddr("controlplane");
        socket.sendTo(packet, dest_address, this->dest_port);
    }
}

void Agent::socketDataArrived(UdpSocket *socket, Packet *packet) {}
void Agent::socketErrorArrived(UdpSocket *socket, Indication *indication) {}
void Agent::socketClosed(UdpSocket *socket) {}

void Agent::handleStartOperation(LifecycleOperation *operation) {
    scheduleAt(simTime(), new cMessage("wait_for_RL"));
}
void Agent::handleStopOperation(LifecycleOperation *operation) {}
void Agent::handleCrashOperation(LifecycleOperation *operation) {}

};
