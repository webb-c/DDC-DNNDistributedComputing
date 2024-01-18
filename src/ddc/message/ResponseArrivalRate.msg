//
// ResponseArrivalRate.msg
//
//  Created on: 2024. 01. 04.
//      Author: Suhwan
//

import inet.common.packet.Packet;

namespace inet;

class ResponseArrivalRate extends inet::Packet { 
    double arrival_rate;
}
