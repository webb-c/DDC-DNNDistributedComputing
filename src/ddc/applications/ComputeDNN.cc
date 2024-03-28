/*
 * ComputeDNN.cc
 *
 *  Created on: 2023. 12. 22.
 *      Author: Suhwan
 */
#include "ComputeDNN.h"

namespace inet {

Define_Module(ComputeDNN);

ComputeDNN::ComputeDNN() {}

ComputeDNN::~ComputeDNN() {}

void ComputeDNN::initialize(int stage) {
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        parseParameters();

    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        init();
    }
}

void ComputeDNN::init() {
    tcp_socket.setOutputGate(gate("socketOut"));
    tcp_socket.bind(L3Address(), this->local_port);
    tcp_socket.setCallback(this);
    tcp_socket.listen(); // for tcp in connection

    socket.setOutputGate(gate("socketOut"));
    socket.bind(L3Address(), this->local_port);

    parseAddress();
    initLayerNodeOfMe();
    initLayerNodePair();
    initQueue();
    initRecorder();
    initProcessingUnit();
}

void ComputeDNN::parseAddress(){
    const char *dest_addresses = par("dest_addresses");
    cStringTokenizer tokenizer(dest_addresses);
    const char *token;

    while ((token = tokenizer.nextToken()) != nullptr) {
        L3Address result;
        L3AddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV_ERROR << "cannot resolve destination address: " << token << endl;
        this->dest_addresses.push_back(result);
        this->dest_addresses_str.push_back(string(token));
    }
}

L3Address ComputeDNN::chooseDestAddr(int next_node_index){
    string address;
    address = "node" + to_string(next_node_index);

    L3Address result;
    L3AddressResolver().tryResolve(address.c_str(), result);
    if (result.isUnspecified())
        EV_ERROR << "cannot resolve destination address: " << address << endl;

    return result;
}

L3Address ComputeDNN::chooseDestAddr(string dst_name) {
    L3Address result;
    L3AddressResolver().tryResolve(dst_name.c_str(), result);
    if (result.isUnspecified())
        EV_ERROR << "cannot resolve destination address: " << dst_name << endl;

    return result;
}

void ComputeDNN::parseParameters(){
    this->node_index = par("node_index").intValue();
    this->layer_depth = par("layer_depth").intValue();
    this->local_port = par("local_port").intValue();
    this->dest_port = par("dest_port").intValue();
    this->controlplane_port = par("controlplane_port").intValue();
    this->virtual_queue_record_duration = par("virtual_queue_record_duration").doubleValue();
    this->bitrate = par("bitrate").doubleValue();
    this->compute_capacity = par("compute_capacity");

    this->node_name = "node" + to_string(this->node_index);
}

void ComputeDNN::initLayerNodeOfMe() {
    for (int depth = 1; depth < this->layer_depth + 1; depth++){
        LayerNode layer_node = LayerNode(this->node_index, depth);
        this->layer_node_vector.push_back(layer_node);
    }
}

void ComputeDNN::initLayerNodePair() {
    for (int depth = 1; depth < this->layer_depth + 1; depth++) {
        LayerNode my_layer_node = LayerNode(this->node_index, depth);

        for (string neighbor_node_name : this->dest_addresses_str) {
            int neighbor_node_index = returnNodeNumberFromNodeName(neighbor_node_name);

            LayerNode neighbor_layer_node = LayerNode(neighbor_node_index, depth);

            LayerNodePair laye_node_pair = LayerNodePair(my_layer_node, neighbor_layer_node);

            this->virtual_links.push_back(laye_node_pair);
        }
    }
    for (int depth = 1; depth < this->layer_depth; depth++) {
        LayerNode layer_node = LayerNode(this->node_index, depth);
        LayerNode layer_node_next_layer = LayerNode(this->node_index, depth + 1);

        LayerNodePair laye_node_pair = LayerNodePair(layer_node, layer_node_next_layer);

        this->virtual_links.push_back(laye_node_pair);
    }
}

void ComputeDNN::initQueue() {
    initVirtualQueue();
    initPhysicalQueue();
}

void ComputeDNN::initVirtualQueue() {
    for (LayerNodePair link : this->virtual_links) {
        this->virtual_queue.addVirtualQueue(link);
    }
}

void ComputeDNN::initPhysicalQueue() {
    PacketQueue *link_packet_queue = dynamic_cast<PacketQueue *>(this->getParentModule()->getSubmodule("wlan", 0)->getSubmodule("queue"));

    this->physical_queue.initLinkPhysicalQueue(link_packet_queue);
}

void ComputeDNN::initRecorder() {
    // init virtual queue recorder of all layer nodes
    for (LayerNodePair virtual_link : this->virtual_links) {
        cOutVector *virtual_queue_recorder = new cOutVector();
        virtual_queue_recorder->setName(("2. layer node virtual backlog: " + virtual_link.toString()).c_str());

        this->layer_node_virtual_queue_recorder_table[virtual_link] = virtual_queue_recorder;
    }

    // init virtual queue recorder of all nodes
    for (string dest_string : this->dest_addresses_str) {
        string recorder_name = this->node_name + "->" + dest_string;
        cOutVector *virtual_queue_recorder = new cOutVector();
        virtual_queue_recorder->setName(("1. node virtual backlog: " + recorder_name).c_str());

        this->node_virtual_queue_recorder_table[dest_string] = virtual_queue_recorder;
    }

    string recorder_name = this->node_name + "->" + this->node_name;
    cOutVector *virtual_queue_recorder = new cOutVector();
    virtual_queue_recorder->setName(("1. node virtual backlog: " + recorder_name).c_str());

    this->node_virtual_queue_recorder_table[this->node_name] = virtual_queue_recorder;

    // init physical queue recorder
    recorder_name = this->node_name;
    cOutVector *physical_queue_recorder = new cOutVector();
    physical_queue_recorder->setName(("3. node physical backlog: " + recorder_name).c_str());

    this->node_physical_queue_recorder = physical_queue_recorder;

    // init ene to end latency recorder
    this->end_to_end_latency_recorder = new cOutVector();
    this->end_to_end_latency_recorder->setName(this->node_name.c_str());
}

void ComputeDNN::initProcessingUnit() {
    this->processing_unit = findProcessingUnit();
}

void ComputeDNN::initTcpSockets() {
    for (const string &node_name : this->dest_addresses_str) {
        L3Address destination = chooseDestAddr(node_name);
        TcpSocket *tcp_socket = new TcpSocket();
        tcp_socket->setCallback(this);
        tcp_socket->connect(destination, this->dest_port);
        tcp_out_socket_map[node_name] = tcp_socket;
    }

}

void ComputeDNN::handleMessageWhenUp(cMessage *msg) {

    if (strcmp(msg->getFullName(), "schedule") == 0) {
        // message from ControlPlane
        Schedule *schedule = dynamic_cast<Schedule *>(msg);
        handleScheduleMessage(schedule);
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
    else if (strcmp(msg->getFullName(), "request_information") == 0) {
        handleRequestInformationMessage();
        delete msg;
    }
    else if (strcmp(msg->getFullName(), "compute_finish") == 0) {
        // message from ProcessingUnit
        ComputeFinish *compute_finish = dynamic_cast<ComputeFinish *>(msg);
        handleComputeFinishMessage(compute_finish);
//        delete msg;
    }
    else {
        TcpSocket *socket = check_and_cast_nullable<TcpSocket *>(tcp_in_socket_map.findSocketFor(msg));
        if (socket)
            socket->processMessage(msg);
        else if (tcp_socket.belongsToSocket(msg))
            tcp_socket.processMessage(msg);
        else if (msg->getKind() == TCP_I_ESTABLISHED){
            EV << "Unknown incoming message: '%s'" << msg->getName();
            delete msg;
        }
        else if (msg->getKind() == TCP_I_CLOSED){
            EV << "Unknown incoming message: '%s'" << msg->getName();
            delete msg;
        }
        else {
//            throw cRuntimeError("Unknown incoming message: '%s'", msg->getName());
            /*bool is_self_msg = msg->isSelfMessage();
            bool is_scheduled = msg->isScheduled();
            string sender_module_name = msg->getSenderModule()->getFullName();
            string sender_gate = msg->getSenderGate()->getFullName();
            string arrival_module = msg->getArrivalModule()->getFullName();
            string arrival_gate = msg->getArrivalGate()->getFullName();int sender_module_id = msg->getSenderModuleId();
            int sender_gate_id = msg->getSenderGateId();
            int arrival_module_id = msg->getArrivalModuleId();
            int arrival_gate_id = msg->getArrivalGateId();
            long id = msg->getId();
            // Assuming you would have handled exceptions elsewhere if msg->getName() is needed
            // throw runtime_error(string("Unknown incoming message:") +  msg->getName());

            string log_message = "is_self_msg: " + to_string(is_self_msg);
            log_message += ", is_scheduled: " + to_string(is_scheduled);
            log_message += ", sender_module_name: " + sender_module_name;
            log_message += ", sender_gate: " + sender_gate;
            log_message += ", arrival_module: " + arrival_module;
            log_message += ", arrival_gate: " + arrival_gate;
            log_message += ", sender_module_id: " + to_string(sender_module_id);
            log_message += ", sender_gate_id: " + to_string(sender_gate_id);
            log_message += ", arrival_module_id: " + to_string(arrival_module_id);
            log_message += ", arrival_gate_id: " + to_string(arrival_gate_id);
            log_message += ", id: " + to_string(id);*/

            // throw runtime_error(string("Unknown incoming message:") +  log_message);
            delete msg;
        }
    }
}

void ComputeDNN::handleScheduleMessage(Schedule *msg) {
    if (this->is_reset) { return; }

    DNNSublayerConfig *dnn_sublayer_config = peekDNNSublayerConfigFromSchedule(msg);
    LayerNodePair layer_node_pair = LayerNodePair(dnn_sublayer_config->getComputingLayerNode(), dnn_sublayer_config->getNextComputingLayerNode());

    // make DNNSublaye from dnn_sublayer_config
    DNNSublayer dnn_sublayer = this->dnn_sublayer_factory.makeDNNSublayer(*dnn_sublayer_config);

    // if sublayer is source, send (compute or send) DNNSublayer
    if (dnn_sublayer_config->isSource()) {
        sendDNNSublayerToNextLayerNode(dnn_sublayer, layer_node_pair);
    }
    else{
        this->dnn_sublayer_table.registerDNNRoute(dnn_sublayer.routeIDToString(), dnn_sublayer.getNextComputingLayerNode());
        this->virtual_queue.pushDNNSublayerToVirtualQueueByLayerNodePair(dnn_sublayer, layer_node_pair);

        recordQueue();

        // register route table and virtual backlog -> wait for precedence DNNOutput
        for (ComputeFinish *compute_finish_message : this->dnn_output_vector) {

            const DNNOutput *dnn_output = peekDNNOutputFromComputeFinish(compute_finish_message);

            // Next computing node of DNNOutput is me, with layer node.
            if (dnn_sublayer_config->IDToString() == dnn_output->IDToString() && dnn_output->getNextComputingLayerNode() == dnn_sublayer_config->getComputingLayerNode()) {
                const auto &position = find(this->dnn_output_vector.begin(), this->dnn_output_vector.end(), compute_finish_message);

                if (position != this->dnn_output_vector.end()) {
                    this->dnn_output_vector.erase(position);
                }

                scheduleAt(simTime(), compute_finish_message);
            }
        }
    }
}

void ComputeDNN::handleComputeFinishMessage(ComputeFinish *msg) {
    if (this->is_reset) { return; }

    const DNNOutput *dnn_output = peekDNNOutputFromComputeFinish(msg);

    // Next computing node of DNNOutput is me, with layer node.
    LayerNode current_node = dnn_output->getNextComputingLayerNode();

    // If next schedule exits
    if (this->dnn_sublayer_table.isRouteIDExist(dnn_output->routeIDToString())) {
        LayerNode next_node = this->dnn_sublayer_table.popNextLayerNodeByRouteID(dnn_output->routeIDToString());
        LayerNodePair layer_node_pair = LayerNodePair(current_node, next_node);

        DNNSublayer dnn_sublayer = this->virtual_queue.popDNNSublayerToVirtualQueueByLayerNodePair(*dnn_output, layer_node_pair);
        recordQueue();

        sendDNNSublayerToNextLayerNode(dnn_sublayer, layer_node_pair);
        delete msg;
    }
    else if (dnn_output->isLast()) { // If last schedule.
        double end_simulation_time = simTime().dbl();
        double end_to_end_latency = dnn_output->getEndToEndLatency(end_simulation_time);
        recordEndToEndLatency(end_to_end_latency);
        delete msg;
    }
    else { // If compute finish is faster then schedule.
        // TODO
        throw runtime_error("this is fast." + dnn_output->serialize());
        this->dnn_output_vector.push_back(msg);
    }

}

void ComputeDNN::handleResetMessage() {
    reset();
}

void ComputeDNN::handleStartMessage() {
    start();
}

void ComputeDNN::handleRequestInformationMessage() {
    map<LayerNodePair, double> links_information;

    string temp = "";

    for (LayerNodePair link : this->virtual_links) {
        links_information[link] = this->virtual_queue.getQueueLengthByLayerNodePair(link);
        temp += link.toString() + to_string(links_information[link]);
    }

    NodeInformation node_information = NodeInformation(this->node_name, links_information);

    Packet *response_node_information = returnResponseNodeInformationMessage(node_information);

    L3Address dest_address = chooseDestAddr("controlplane");
    socket.sendTo(response_node_information, dest_address, this->controlplane_port);
}

Packet *ComputeDNN::returnResponseNodeInformationMessage(NodeInformation &node_information) {
    Packet *response_node_information = new Packet("response_node_information");

    string node_information_serialized = node_information.serialize();
    vector<uint8_t> node_information_serialized_vector(node_information_serialized.begin(), node_information_serialized.end());
    const auto& node_information_chunk = makeShared<BytesChunk>(node_information_serialized_vector);

    response_node_information->insertAtBack(node_information_chunk);

    return response_node_information;
}

void ComputeDNN::reset() {
    EV << "reset compute dnn";
    this->is_reset = true;
    this->virtual_queue.resetVirtualQueue();
    initVirtualQueue();
    recordQueue();

    cMessage* reset_msg = new cMessage("reset");
    sendDirect(reset_msg, this->processing_unit, "dataIn");
}

void ComputeDNN::start() {
    EV << "start compute dnn";
    this->is_reset = false;
}

void ComputeDNN::recordQueue() {
    // recordVirtualQueue();
    recordPysicalQueue();
}

void ComputeDNN::recordVirtualQueue() {
    if (this->layer_node_virtual_queue_recorder_table.size() == 0){
        throw runtime_error("Please initialize virtual_queue_recorder_table first.");
    }
    else {
        map<string, double> node_virtual_backlogs;

        for (LayerNodePair virtual_link : this->virtual_links) {
            double layer_node_virtual_backlog = this->virtual_queue.getQueueLengthByLayerNodePair(virtual_link);

            cOutVector *layer_node_virtual_queue_recorder = this->layer_node_virtual_queue_recorder_table[virtual_link];
            layer_node_virtual_queue_recorder->record(layer_node_virtual_backlog);

            node_virtual_backlogs[virtual_link.getDstLayerNode().toNodeName()] += layer_node_virtual_backlog;
        }

        for (string dest_string : this->dest_addresses_str) {
            double node_virtual_backlog = node_virtual_backlogs.at(dest_string);

            cOutVector *node_virtual_queue_recorder = this->node_virtual_queue_recorder_table[dest_string];
            node_virtual_queue_recorder->record(node_virtual_backlog);
        }

        double node_virtual_backlog = node_virtual_backlogs.at(this->node_name);

        cOutVector *node_virtual_queue_recorder = this->node_virtual_queue_recorder_table[this->node_name];
        node_virtual_queue_recorder->record(node_virtual_backlog);
    }
}

void ComputeDNN::recordPysicalQueue() {
    if (!this->physical_queue.isPhysicalQueueExist()) {
        throw runtime_error("Please initialize physical_queue first.");
    }
    else {
        double node_physical_backlog = this->physical_queue.getQueueLength() + this->processing_unit->getComputingQueueLength();
        this->node_physical_queue_recorder->record(node_physical_backlog);
    }
}

void ComputeDNN::recordEndToEndLatency(double latency) {
    this->end_to_end_latency_recorder->record(latency);
}

ProcessingUnit* ComputeDNN::findProcessingUnit() {
    return dynamic_cast<ProcessingUnit *>(getParentModule()->getSubmodule("app", 1));
}

void ComputeDNN::sendComputeMessage(DNNSublayer dnn_sublayer) {
    Compute* compute_msg = new Compute("compute");
    compute_msg->setDnn_sublayer(dnn_sublayer.deepcopy()); // TODO deep copy layer.
    sendDirect(compute_msg, this->processing_unit, "dataIn");
}

void ComputeDNN::sendDNNSublayerToNextLayerNode(DNNSublayer dnn_sublayer, LayerNodePair layer_node_pair) {
    if (layer_node_pair.isSameLayer() && !layer_node_pair.isSameNode()){ // same layer, different node -> data transmit
        sendDNNSublayerToNextNode(dnn_sublayer);
    }
    else if (layer_node_pair.isSameNode() && !layer_node_pair.isSameLayer()){ // different layer, same node -> compute
        sendComputeMessage(dnn_sublayer);
    }
}

void ComputeDNN::sendDNNSublayerToNextNode(DNNSublayer dnn_sublayer) {
    DNNOutput dnn_output = dnn_sublayer.getOutput();
    LayerNode next_layer_node = dnn_sublayer.getNextComputingLayerNode();

    Packet *send_start_msg = new Packet("send_start");
    Packet *compute_finish_msg = new Packet("compute_finish");
    Packet *send_finish_msg = new Packet("send_finish");

    string send_start_delimeter = "<send>";
    string dnn_output_serialized = dnn_output.serialize();
    string send_finish_delimeter = ">send<";

    vector<uint8_t> send_start_delimeter_vector(send_start_delimeter.begin(), send_start_delimeter.end());
    const auto& send_start_delimeter_chunk = makeShared<BytesChunk>(send_start_delimeter_vector);

    vector<uint8_t> dnn_output_serialized_vector(dnn_output_serialized.begin(), dnn_output_serialized.end());
    const auto& dnn_output_chunk = makeShared<BytesChunk>(dnn_output_serialized_vector);

    vector<uint8_t> send_finish_delimeter_vector(send_finish_delimeter.begin(), send_finish_delimeter.end());
    const auto& send_finish_delimeter_chunk = makeShared<BytesChunk>(send_finish_delimeter_vector);

    send_start_msg->insertAtBack(send_start_delimeter_chunk);
    compute_finish_msg->insertAtBack(dnn_output_chunk);
    send_finish_msg->insertAtBack(send_finish_delimeter_chunk);

    if (tcp_out_socket_map.count(next_layer_node.toNodeName()) == 0) {
        L3Address destination = chooseDestAddr(next_layer_node.toNodeName());
        TcpSocket *tcp_socket = new TcpSocket();
        tcp_socket->setOutputGate(gate("socketOut"));
        tcp_socket->setCallback(this);
        tcp_socket->connect(destination, this->dest_port);
        tcp_out_socket_map[next_layer_node.toNodeName()] = tcp_socket;
    }

    tcp_out_socket_map.at(next_layer_node.toNodeName())->send(send_start_msg);
    tcp_out_socket_map.at(next_layer_node.toNodeName())->send(compute_finish_msg);
    tcp_out_socket_map.at(next_layer_node.toNodeName())->send(send_finish_msg);
}

DNNSublayerConfig *ComputeDNN::peekDNNSublayerConfigFromSchedule(Schedule *msg) {
    DNNSublayerConfig *dnn_sublayer_config = new DNNSublayerConfig();

    auto data_chunk = msg->peekDataAsBytes();
    const auto& byte_array = data_chunk->getBytes();
    string json_string(byte_array.begin(), byte_array.end());

    dnn_sublayer_config->deserialize(json_string);

    return dnn_sublayer_config;
}

const DNNOutput *ComputeDNN::peekDNNOutputFromComputeFinish(ComputeFinish *msg) {
    return msg->getDnn_output();
}

double ComputeDNN::getQueueLengthByLayerNodePair(LayerNodePair link) {
    return this->virtual_queue.getQueueLengthByLayerNodePair(link);
}

void ComputeDNN::socketAvailable(TcpSocket *socket, TcpAvailableInfo *available_info) {
    // new TCP connection -- create new socket object and server process
    TcpSocket *new_socket = new TcpSocket(available_info);
    new_socket->setOutputGate(gate("socketOut"));
    new_socket->setCallback(this);

    this->tcp_in_socket_map.addSocket(new_socket);

    socket->accept(available_info->getNewSocketId());
}

void ComputeDNN::socketDataArrived(TcpSocket *socket, Packet *packet, bool urgent) {
    int socket_id = socket->getSocketId();

    auto data_chunk = packet->peekDataAsBytes();
    const auto& byte_array = data_chunk->getBytes();
    string json_string(byte_array.begin(), byte_array.end());

    this->socket_data_queue[socket_id] += json_string;

    while (true) {
        string data_stream = this->socket_data_queue[socket_id];

        if (data_stream.find(START_DELIMITER) != string::npos && data_stream.find(END_DELIMITER) != string::npos) { // when >send< in text at least one data completely come.
            if (data_stream.find(START_DELIMITER) > data_stream.find(END_DELIMITER)) { throw runtime_error("Data stream is weird : " + data_stream); }

            int start_index = data_stream.find(START_DELIMITER);
            int finish_index = data_stream.find(END_DELIMITER);

            this->socket_data_queue[socket_id] = data_stream.substr(finish_index + END_DELIMITER.size()); // cut data

            string data_refined = data_stream.substr(0, finish_index);
            data_refined = data_refined.substr(start_index + START_DELIMITER.size());

            DNNOutput *dnn_output = new DNNOutput;
            ComputeFinish *compute_finish = new ComputeFinish();

            dnn_output->deserialize(data_refined);

            compute_finish->setDnn_output(dnn_output);

            handleComputeFinishMessage(compute_finish);
        }
        else {
            break;
        }
    }


    delete packet;


}

void ComputeDNN::handleStartOperation(LifecycleOperation *operation) {}
void ComputeDNN::handleStopOperation(LifecycleOperation *operation) {}
void ComputeDNN::handleCrashOperation(LifecycleOperation *operation) {}



} //namespace inet
