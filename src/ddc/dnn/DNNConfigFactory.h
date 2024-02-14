/*
 * DNNConfigFactory.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#ifndef __DDC_DNNCONFIGFACTORY_H
#define __DDC_DNNCONFIGFACTORY_H

#include "ddc/libs/nlohmann/json.hpp"
#include "ddc/dnn/DNNConfig.h"

#include <iostream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <fstream>
#include <winsock2.h>
#include <windows.h>


using namespace std;
using json = nlohmann::json;

namespace inet{

class DNNConfigFactory
{
  private:
    unordered_map<string, string> dnn_config_paths;

    string dnn_config_path;
    string dnn_config_json_string;
    json dnn_config_json;
    DNNConfig dnn_config;

    void loadPathFromModelName(string dnn_model_name);
    void loadJsonString();
    void loadJson();
    void buildDNNConfig(double input_size);
    string exePath();

  public:
    DNNConfigFactory();

    DNNConfig makeDNNConfig(string dnn_model_name, double input_size);
};

}

#endif // ifndef __DDC_DNNCONFIGFACTORY_H
