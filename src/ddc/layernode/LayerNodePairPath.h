/*
 * LayerNodePairPath.h
 *
 *  Created on: 2023. 12. 28.
 *      Author: Suhwan
 */

#ifndef __LAYERNODEPAIRPATH_H_
#define __LAYERNODEPAIRPATH_H_

#include <iostream>
#include <vector>

#include "ddc/layernode/LayerNodePair.h"
#include "ddc/layernode/LayerNode.h"

using namespace std;

namespace inet{

class LayerNodePairPath {
    private:
        int index;
        int path_length;
        vector<LayerNodePair> dnn_paths;

    public:
        LayerNodePairPath(vector<LayerNode> paths);

        void initialize(vector<LayerNode> paths);

        void resetIndex();
        void increaseIndex();
        int getPathLength() { return this->dnn_paths.size(); }
        LayerNodePair next();

};

}



#endif /* __LAYERNODEPAIRPATH_H_ */
