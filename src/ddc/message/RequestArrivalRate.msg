//
// RequestArrivalRate.msg
//
//  Created on: 2024. 01. 04.
//      Author: Suhwan
//

import inet.common.packet.Packet;


cplusplus {{
#include "ddc/layernode/LayerNode.h"
}}

namespace inet;


class LayerNode{
    @existingClass;
}

class RequestArrivalRate extends inet::Packet { 
    string agent_name;
    string callback_node_name;
    int callback_port;
    LayerNode src_layer_node;
    LayerNode dst_layer_node;
}
