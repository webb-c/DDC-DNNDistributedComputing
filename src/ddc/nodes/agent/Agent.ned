//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package ddc.nodes.agent;

import inet.applications.contract.IApp;

simple Agent like IApp
{
    parameters:
        string dest_addresses;
        int layer_depth;
        int local_port = default(5001);
        int dest_port = default(5000);
        string pipe_name = default("\\\\.\\pipe\\frame_drop_rl");
        double skip_length;
        string src_node_name;
        string dst_node_name;
        string model_name;
        string agent_name;
        double input_size;
     
    gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

