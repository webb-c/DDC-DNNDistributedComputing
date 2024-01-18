/*
 * DNNID.cpp
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#include "ddc/dnn/DNNID.h"

namespace inet
{   
    DNNID::DNNID() : DNNID::DNNID("model_1", "agent_1", 0.0, -1){
    }

    DNNID::DNNID(string dnn_model_name, string agent_name, double start_simulation_time, double dnn_identifier){
        this->dnn_model_name = dnn_model_name;
        this->agent_name = agent_name;
        this->start_simulation_time = start_simulation_time;
        this->dnn_identifier = dnn_identifier;
    }

    string DNNID::toString() const{
        return "model_name:" + this->dnn_model_name + ", agent_name:" + this->agent_name + ", dnn_identifier:" + to_string(this->dnn_identifier);
    }

    bool DNNID::operator==(const DNNID& other) const{
        if (other.getAgentName() == this->agent_name &&
            other.getDNNModelName() == this->dnn_model_name &&
            other.getDNNIdentifier() == this->dnn_identifier)
            return true;
        else
            return false;
    }

    string DNNID::serialize() const {
        json j;

        j["agent_name"] = this->agent_name;
        j["dnn_model_name"] = this->dnn_model_name;
        j["start_simulation_time"] = this->start_simulation_time;
        j["dnn_identifier"] = this->dnn_identifier;

        return j.dump();
    }

    void DNNID::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->agent_name = j["agent_name"];
        this->dnn_model_name = j["dnn_model_name"];
        this->start_simulation_time = j["start_simulation_time"];
        this->dnn_identifier = j["dnn_identifier"];
    }



} // namespace dnn
