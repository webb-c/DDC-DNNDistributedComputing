/*
 * DNNConfigFactory.cc
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNConfigFactory.h"
#include <omnetpp.h>
using namespace omnetpp;
namespace inet{

    DNNConfigFactory::DNNConfigFactory() {
        string root = exePath();
//        throw runtime_error(root + "../ddc/dnn/dnn_config/model_1.json");
        this->dnn_config_paths = unordered_map<string, string>{
            {"model_1", root + "/../ddc/dnn/dnn_config/model_1.json"},
            {"model_2", root + "/../ddc/dnn/dnn_config/model_2.json"},
            {"model_3", root + "/../ddc/dnn/dnn_config/model_3.json"},
            {"SLN", root + "/../ddc/dnn/dnn_config/SLN.json"},
            {"ELN", root + "/../ddc/dnn/dnn_config/ELN.json"},
            {"YOLO", root + "/../ddc/dnn/dnn_config/YOLO.json"},
          };
    }

    DNNConfig DNNConfigFactory::makeDNNConfig(string dnn_model_name){
        loadPathFromModelName(dnn_model_name);
        loadJsonString();
        loadJson();
        buildDNNConfig();

        return this->dnn_config;
    }

    void DNNConfigFactory::loadPathFromModelName(string dnn_model_name){
        this->dnn_config_path = this->dnn_config_paths[dnn_model_name];
    }

    void DNNConfigFactory::loadJsonString(){
        ifstream dnn_config_ifstream(this->dnn_config_path);
        if (!dnn_config_ifstream) {
            throw runtime_error("Failed to open file: " + dnn_config_path);
        }
        string content((istreambuf_iterator<char>(dnn_config_ifstream)), istreambuf_iterator<char>());

        this->dnn_config_json_string = content;
    }

    void DNNConfigFactory::loadJson(){
        this->dnn_config_json = json::parse(this->dnn_config_json_string);
    }

    void DNNConfigFactory::buildDNNConfig(){
        this->dnn_config = DNNConfig(this->dnn_config_json);
    }

    string DNNConfigFactory::exePath() {
        char buffer[MAX_PATH];
        GetModuleFileName( NULL, buffer, MAX_PATH );
        return string(buffer);
    }
}

