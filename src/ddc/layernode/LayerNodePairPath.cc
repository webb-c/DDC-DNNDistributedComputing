/*
 * LayerNodePairPath.cc
 *
 *  Created on: 2023. 12. 28.
 *      Author: Suhwan
 */

#include "LayerNodePairPath.h"

namespace inet {

    LayerNodePairPath::LayerNodePairPath(vector<LayerNode> path){
        this->initialize(path);
    }

    void LayerNodePairPath::initialize(vector<LayerNode> path){
        this->dnn_paths.clear();
        this->resetIndex();

        int path_length = path.size();

        for (int i = 0; i < path_length - 1; i++){
            LayerNodePair dnn_path = LayerNodePair(path[i], path[i+1]);
            this->dnn_paths.push_back(dnn_path);
        }
    }

    void LayerNodePairPath::resetIndex(){
        this->index = 0;
    }

    void LayerNodePairPath::increaseIndex(){
        this->index += 1;
    }

    LayerNodePair LayerNodePairPath::next(){
        LayerNodePair layer_node_current_index = this->dnn_paths[this->index];
        this->increaseIndex();

        return layer_node_current_index;
    }
}
