/*
 * DNNConfig.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNCONFIG_H
#define __DNN_DNNCONFIG_H

#define LINK_INDEX 0
#define COMPUTING_INDEX 1

#include "ddc/libs/nlohmann/json.hpp"

#include <iostream>
#include <unordered_map>
#include <fstream>

using namespace std;
using json = nlohmann::json;

namespace inet{

class DNNConfig
{
  private:
    string name;
    int sublayer_num;
    vector<vector<double>> link_and_computing_config;
    vector<double> link_config_vector;
    vector<double> computing_config_vector;

  public:
    DNNConfig();
    DNNConfig(json json_config);
    
    string getName() { return this->name; }
    int getSublayerNum() { return this->sublayer_num; }
    vector<vector<double>> getLinkAndComputingConfig() { return this->link_and_computing_config; }
    vector<double> getLinkConfigVector() { return this->link_config_vector; }
    vector<double> getComputingConfigVector() { return this->computing_config_vector; }

    double getLinkByLayerIndex(int layer_index);
    double getComputingByLayerIndex(int layer_index);
    
    json createDefaultJson();

};

}

#endif // ifndef __DNN_DNNFACTORY_H
