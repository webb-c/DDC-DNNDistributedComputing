//
// ComputeFinish.msg
//
//  Created on: 2024. 01. 01.
//      Author: Suhwan
//

import inet.common.packet.Packet;

cplusplus {{
#include "ddc/dnn/DNNOutput.h"
}}

namespace inet;


class DNNOutput{
    @existingClass;
}

class ComputeFinish extends inet::Packet { 
    DNNOutput *dnn_output;
}
