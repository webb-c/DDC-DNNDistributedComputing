/*
 * DNNRouteID.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DDC_DNNROUTEID_H
#define __DDC_DNNROUTEID_H

#include <iostream>
#include <string>

#include "ddc/libs/nlohmann/json.hpp"

#include "ddc/dnn/DNNID.h"
#include "ddc/layernode/LayerNode.h"

using namespace std;
using json = nlohmann::json;

namespace inet {

class DNNRouteID
{
    protected:
        DNNID dnn_id;
        LayerNode layer_node;

    public:
        DNNRouteID(); 
        DNNRouteID(DNNID dnn_id, LayerNode layer_node);

        string toString() const;

        string serialize() const;
        void deserialize(string json_string);
};

}

#endif // ifndef __DDC_DNNROUTEID_H
