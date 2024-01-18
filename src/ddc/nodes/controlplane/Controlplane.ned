//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package ddc.nodes.controlplane;

import inet.applications.contract.IApp;

simple Controlplane like IApp
{
    parameters:
        int layer_depth;
        int node_num;
        int local_port = default(5000);
        int dest_port = default(5000);
        string routing_algorithm = default("dijkstra");
        string dest_addresses;
        
		int timeToLive = default(-1); // if not -1, set the TTL (IPv4) or Hop Limit (IPv6) field of sent packets to this value
		int dscp = default(-1); // if not -1, set the DSCP field (on IPv4/IPv6) of sent packets to this value
		string multicastInterface = default("");  // if not empty, set the multicast output interface option on the socket (interface name expected)
		string interfaceTableModule;   // The path to the InterfaceTable module
		bool receiveBroadcast = default(false); // if true, makes the socket receive broadcast packets
        bool joinLocalMulticastGroups = default(false); // if true, makes the socket receive packets from all multicast groups set on local interfaces

    gates:
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

