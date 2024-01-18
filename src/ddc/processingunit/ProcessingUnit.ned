//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package ddc.processingunit;

import inet.applications.contract.IApp;


simple ProcessingUnit like IApp
{
    parameters:
        double compute_capacity = default(20000000); // 20000000Flops
        int local_port = default(4999);
        
    gates:
        input dataIn;
        input socketIn @labels(UdpControlInfo/up);
        output socketOut @labels(UdpControlInfo/down);
}

