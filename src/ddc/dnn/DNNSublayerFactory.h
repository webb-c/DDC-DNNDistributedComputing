/*
 * DNNSublayerFactory.h
 *
 *  Created on: 2023. 12. 29.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNSUBLAYERFACTORY_H
#define __DNN_DNNSUBLAYERFACTORY_H

#include "ddc/dnn/DNNSublayerConfig.h"
#include "ddc/dnn/DNNRouteID.h"
#include "ddc/dnn/DNNSublayer.h"
#include "ddc/dnn/DNNConfigFactory.h"

#include <iostream>

using namespace std;

namespace inet{

class DNNSublayerFactory
{
  private:
    DNNConfigFactory dnn_config_factory;

    string model_name;
    int sublayer_index;
    bool need_computing;
    DNNID dnn_id;

    DNNConfig dnn_config;

    double link;
    double computing;

    bool is_source = false;
    bool is_last = false;

    LayerNode computing_layer_node;
    LayerNode next_computing_layer_node;

    DNNSublayer dnn_sublayer;

    void unzipDNNSublayerConfig(DNNSublayerConfig dnn_sublayer_config);
    void includeDNNConfig();
    void loadLinkAndComputing();
    void loadLink();
    void loadComputing();
    void buildDNNSublayer();
    
  public:
    DNNSublayerFactory();

    DNNSublayer makeDNNSublayer(DNNSublayerConfig dnn_sublayer_config);

};

}

#endif // ifndef __DNN_DNNSUBLAYERFACTORY_H
