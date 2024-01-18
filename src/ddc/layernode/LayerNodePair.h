/*
 * LayerNode.h
 *
 *  Created on: 2023. 12. 27.
 *      Author: Suhwan
 */

#ifndef __LAYERNODEPAIR_H_
#define __LAYERNODEPAIR_H_

#include <iostream>

#include <ddc/layernode/LayerNode.h>

using namespace std;

namespace inet{
    
class LayerNodePair {
    private:
        LayerNode src_layer_node;
        LayerNode dst_layer_node;

    public:
        LayerNodePair();
        LayerNodePair(LayerNode src_layer_node,  LayerNode dst_layer_node);
        string toString() const;

        LayerNode getSrcLayerNode() const;
        LayerNode getDstLayerNode() const;

        bool operator<(const LayerNodePair& target_layer_node_pair) const;
        bool operator==(const LayerNodePair& target_layer_node_pair) const;

        bool isSameLayer();
        bool isSameNode();

        string serialize() const;
        void deserialize(string json_string);
};
}



#endif /* __LAYERNODEPAIR_H_ */
