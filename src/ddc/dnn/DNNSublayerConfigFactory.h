/*
 * DNNSublayerConfigFactory.h
 *
 *  Created on: 2024. 01. 03.
 *      Author: Suhwan
 */

#ifndef __DDC_DNNSUBLAYERCONFIGFACTORY_H
#define __DDC_DNNSUBLAYERCONFIGFACTORY_H

#include <iostream>
#include <vector>

#include "ddc/dnn/DNNID.h"
#include "ddc/dnn/DNNSublayerConfig.h"
#include "ddc/dnn/DNNInformation.h"
#include "ddc/layernode/LayerNode.h"
#include "ddc/layernode/LayerNodePair.h"
#include "ddc/layernode/LayerNodePairPath.h"

using namespace std;

namespace inet{

class DNNSublayerConfigFactory
{
  private:
    vector<DNNSublayerConfig *> dnn_sublayer_config_vector;
    
  public:
    DNNSublayerConfigFactory();

    void resetDNNSublayerConfigVector();
    vector<DNNSublayerConfig *> makeDNNSublayerConfigVector(DNNInformation *dnn_information, LayerNodePairPath &layer_node_pair_path);
};

}

#endif // ifndef __DDC_DNNSUBLAYERCONFIGFACTORY_H
