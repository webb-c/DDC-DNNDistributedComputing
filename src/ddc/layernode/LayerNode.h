/*
 * LayerNode.h
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#ifndef __LAYERNODE_H_
#define __LAYERNODE_H_

#include <iostream>
#include <string>
#include <omnetpp.h>

#include "ddc/libs/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace inet{
    
class LayerNode {
    private:
        int node_number;
        int layer_number;

    public:
        LayerNode();
        LayerNode(int node_number, int layer_number);

        bool isSameLayer(LayerNode target_layer_node) const;
        bool isSameNode(LayerNode target_layer_node) const;
        bool isSameLayerNode(LayerNode target_layer_node) const;
        string toString() const;
        string toNodeName() const;

        int getNodeNumber() const;
        int getLayerNumber() const;

        bool operator<(const LayerNode& target_layer_node) const;

        bool operator==(const LayerNode& target_layer_node) const;
        bool operator!=(const LayerNode& target_layer_node) const;

        string serialize() const;
        void deserialize(string json_string);
};

}



#endif /* __LAYERNODE_H_ */
