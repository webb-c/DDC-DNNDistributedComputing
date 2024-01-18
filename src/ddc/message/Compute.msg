//
// Compute.msg
//
//  Created on: 2024. 01. 01.
//      Author: Suhwan
//

cplusplus {{
#include "ddc/dnn/DNNSublayer.h"
}}

namespace inet;


class DNNSublayer{
    @existingClass;
}

packet Compute {
    DNNSublayer *dnn_sublayer;
}
