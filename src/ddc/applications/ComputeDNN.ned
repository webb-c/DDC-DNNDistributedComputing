//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

package ddc.applications;

import inet.applications.contract.IApp;

simple ComputeDNN like IApp
{
    parameters:
    	int node_index;
    	int layer_depth;
		int local_port = default(5000);
		int dest_port = default(5000);
		int controlplane_port = default(5000);
		string dest_addresses;
		double virtual_queue_record_duration = default(0.1);
		double bitrate @unit(bps);
		double compute_capacity = default(20000000);
		
    gates:
        input dataIn;
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}
