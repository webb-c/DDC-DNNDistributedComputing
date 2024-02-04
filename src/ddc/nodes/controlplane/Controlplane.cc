/*
 * Controlplane.cc
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#include "Controlplane.h"

namespace inet {

Define_Module(Controlplane);

Controlplane::Controlplane() {}

Controlplane::~Controlplane() {}

void Controlplane::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        parseParameters();
    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        init();
    }
}

void Controlplane::init() {
    socket.setOutputGate(gate("socketOut"));
    socket.bind(L3Address(), this->local_port);

    parseAddress();
    initDataplane();
    loadAndinitLayeredGraph();

    scheduleAt(simTime(), this->virtual_queue_sync_message);
    this->layered_graph.initRecorder();
}

void Controlplane::parseAddress(){
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

L3Address Controlplane::chooseDestAddr(int next_node_index){
    string address;
    address = "node" + to_string(next_node_index);

    L3Address result;
    L3AddressResolver().tryResolve(address.c_str(), result);
    if (result.isUnspecified())
        EV_ERROR << "cannot resolve destination address: " << address << endl;

    return result;
}

L3Address Controlplane::chooseDestAddr(string name){
    L3Address result;
    L3AddressResolver().tryResolve(name.c_str(), result);
    if (result.isUnspecified())
        EV_ERROR << "cannot resolve destination address: " << name << endl;

    return result;
}

void Controlplane::parseParameters(){
    this->layer_depth = par("layer_depth").intValue();
    this->node_num = par("node_num").intValue();
    this->local_port = par("local_port").intValue();
    this->dest_port = par("dest_port").intValue();
    this->routing_algorithm = par("routing_algorithm").stdstringValue();
}

void Controlplane::loadAndinitLayeredGraph(){
    loadLayeredGraph();
    initLayeredGraph();
}

void Controlplane::loadLayeredGraph(){
    // TODO change to RoutingAlgorithmFactory
    if (this->routing_algorithm == "dijkstra")
        this->layered_graph = LayeredGraph(this->layer_depth, this->node_num, new Dijkstra());

}

void Controlplane::initDataplane() {
    for (string node_name : this->dest_addresses_str) {
        cModule *node = getParentModule()->getParentModule()->getSubmodule(node_name.c_str());
        this->dest_nodes.push_back(node);
    }
}

void Controlplane::initLayeredGraph(){
    for (cModule * node : this->dest_nodes){
        this->layered_graph.registerNode(node);
    }

    // init�� ���������̱� ������ ���� �����ؼ� �ʱ�ȭ
    this->layered_graph.buildAdjacencyMatrix();
}

void Controlplane::updateLayeredGraph(vector<DNNSublayerConfig *> dnn_sublayer_config_vector) {
    vector<DNNSublayer> dnn_sublayer_vector;
    for (const DNNSublayerConfig * dnn_sublayer_config : dnn_sublayer_config_vector) {
        DNNSublayer dnn_sublayer = this->dnn_sublayer_factory.makeDNNSublayer(*dnn_sublayer_config);
        dnn_sublayer_vector.push_back(dnn_sublayer);
    }

    this->layered_graph.applyVirtualUpdate(dnn_sublayer_vector);
}

void Controlplane::handleMessageWhenUp(cMessage *msg) {
    if (strcmp(msg->getFullName(), "route") == 0) {
        Route *route = dynamic_cast<Route *>(msg);
        handleRouteMessage(route);
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "request_arrival_rate") == 0) {
        RequestArrivalRate *request_arrival_rate = dynamic_cast<RequestArrivalRate *>(msg);
        handleArrivalRateMessage(request_arrival_rate);
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "response_node_information") == 0) {
        // message from DNNCompute
        Packet *response_node_information = dynamic_cast<Packet *>(msg);
        handleNodeInformationMessage(response_node_information);
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "reset") == 0) {
        handleResetMessage();
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "start") == 0) {
        handleStartMessage();
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "virtual_queue_sync") == 0) {
        handleVirtualQueueSyncMessage();
    }
}

void Controlplane::handleRouteMessage(Route *msg) {
    UpdateandRecordVirtualQueue();

    DNNInformation *dnn_information = peekDNNInformationFromRoute(msg);

    LayerNode src_layer_node = dnn_information->getSrcLayerNode();
    LayerNode dst_layer_node = dnn_information->getDstLayerNode();

    vector<LayerNode> path = this->layered_graph.findPath(src_layer_node, dst_layer_node);
    LayerNodePairPath layer_node_pair_path = LayerNodePairPath(path);

    vector<DNNSublayerConfig *> dnn_sublayer_config_vector = this->dnn_sublayer_config_factory.makeDNNSublayerConfigVector(dnn_information, layer_node_pair_path);

    updateLayeredGraph(dnn_sublayer_config_vector);

    for (DNNSublayerConfig *dnn_sublayer_config : dnn_sublayer_config_vector) {
        Schedule *schedule = returnScheduleMessage(dnn_sublayer_config);
        LayerNode computing_layer_node = dnn_sublayer_config->getComputingLayerNode();
        L3Address dest_address = chooseDestAddr(computing_layer_node.getNodeNumber());
        socket.sendTo(schedule, dest_address, this->dest_port);
    }
}

Schedule *Controlplane::returnScheduleMessage(DNNSublayerConfig *dnn_sublayer_config) {
    Schedule *schedule = new Schedule("schedule");

    string dnn_sublayer_config_serialized = dnn_sublayer_config->serialize();
    vector<uint8_t> dnn_sublayer_config_serialized_vector(dnn_sublayer_config_serialized.begin(), dnn_sublayer_config_serialized.end());
    const auto& dnn_sublayer_config_chunk = makeShared<BytesChunk>(dnn_sublayer_config_serialized_vector);

    schedule->insertAtBack(dnn_sublayer_config_chunk);

    return schedule;
}

void Controlplane::handleNodeInformationMessage(Packet *response_node_information) {
    NodeInformation node_information = peekNodeInformationFromRoute(response_node_information);

    this->layered_graph.sync(node_information);
}

DNNInformation *Controlplane::peekDNNInformationFromRoute(Route *msg){
    DNNInformation *dnn_information = new DNNInformation();

    auto data_chunk = msg->peekDataAsBytes();
    const auto& byte_array = data_chunk->getBytes();
    string json_string(byte_array.begin(), byte_array.end());

    dnn_information->deserialize(json_string);

    return dnn_information;
}

NodeInformation Controlplane::peekNodeInformationFromRoute(Packet *response_node_information){
    NodeInformation node_information;

    auto data_chunk = response_node_information->peekDataAsBytes();
    const auto& byte_array = data_chunk->getBytes();
    string json_string(byte_array.begin(), byte_array.end());

    node_information.deserialize(json_string);

    return node_information;
}

void Controlplane::handleArrivalRateMessage(RequestArrivalRate *request_arrival_rate) {
    UpdateandRecordVirtualQueue();

    LayerNode src_layer_node = request_arrival_rate->getSrc_layer_node();
    LayerNode dst_layer_node = request_arrival_rate->getDst_layer_node();
    string response_agent_name = request_arrival_rate->getAgent_name();
    string callback_node_name = request_arrival_rate->getCallback_node_name();
    int callback_agent_port = request_arrival_rate->getCallback_port();

    double arrival_rate = this->layered_graph.getArrivalRate(src_layer_node, dst_layer_node);

    ResponseArrivalRate *response_arrival_rate = new ResponseArrivalRate("response_arrival_rate");
    response_arrival_rate->setArrival_rate(arrival_rate);

    addDummyPayloadToPacket(response_arrival_rate, 8);

    L3Address dest_address = chooseDestAddr(callback_node_name);
    socket.sendTo(response_arrival_rate, dest_address, callback_agent_port);
}

void Controlplane::handleResetMessage() {
    reset();
}

void Controlplane::handleStartMessage() {
    start();
}

void Controlplane::reset() {
    EV << "reset control plane";
    sendResetMessageToNodes();
    this->is_reset = true;
}

void Controlplane::start() {
    EV << "start control plane";
    sendStartMessageToNodes();
    this->is_reset = false;
}

void Controlplane::handleVirtualQueueSyncMessage() {
    this->layered_graph.resetNodeSyncIndicator();
    sendReqeustInformationMessageToNodes();

    scheduleAt(simTime() + this->sync_message_duration, this->virtual_queue_sync_message);
}

void Controlplane::UpdateandRecordVirtualQueue() {
    this->layered_graph.update();
    this->layered_graph.recordVirtualQueue();
}

void Controlplane::sendResetMessageToNodes() {
    for (L3Address dest_address : this->dest_addresses) {
        Reset* reset_message = new Reset("reset");

        addDummyPayloadToPacket(reset_message, 1);

        socket.sendTo(reset_message, dest_address, this->dest_port);
    }
}

void Controlplane::sendStartMessageToNodes() {
    for (L3Address dest_address : this->dest_addresses) {
        Reset* start_message = new Reset("start");

        addDummyPayloadToPacket(start_message, 1);

        socket.sendTo(start_message, dest_address, this->dest_port);
    }
}

void Controlplane::sendReqeustInformationMessageToNodes() {
    for (L3Address dest_address : this->dest_addresses) {
        RequestNodeInformation* request_information_message = new RequestNodeInformation("request_information");

        addDummyPayloadToPacket(request_information_message, 1);

        socket.sendTo(request_information_message, dest_address, this->dest_port);
    }
}

void Controlplane::socketDataArrived(UdpSocket *socket, Packet *packet) {}
void Controlplane::socketErrorArrived(UdpSocket *socket, Indication *indication) {}
void Controlplane::socketClosed(UdpSocket *socket) {}

void Controlplane::handleStartOperation(LifecycleOperation *operation) {}
void Controlplane::handleStopOperation(LifecycleOperation *operation) {}
void Controlplane::handleCrashOperation(LifecycleOperation *operation) {}

};
