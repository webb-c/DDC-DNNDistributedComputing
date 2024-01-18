/*
 * DNNInformation.h
 *
 *  Created on: 2023. 12. 31.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNINFORMATION_H
#define __DNN_DNNINFORMATION_H

#include <iostream>
#include <cstdint>

#include "ddc/libs/nlohmann/json.hpp"

#include "ddc/dnn/DNNID.h"
#include "ddc/layernode/LayerNode.h"

using namespace std;
using json = nlohmann::json;

namespace inet {

class DNNInformation
{
    private:
        DNNID dnn_id;
        LayerNode src_layer_node;
        LayerNode dst_layer_node;


    public:
        DNNInformation();
        DNNInformation(DNNID dnn_id, LayerNode src_layer_node, LayerNode dst_layer_node);

        DNNID getDNNID() const { return this->dnn_id; }
        LayerNode getSrcLayerNode() const { return this->src_layer_node; }
        LayerNode getDstLayerNode() const { return this->dst_layer_node; }

        string serialize() const;
        void deserialize(string json_string);
};

}

#endif // ifndef __DNN_DNNINFORMATION_H
