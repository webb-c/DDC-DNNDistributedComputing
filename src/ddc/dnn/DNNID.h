/*
 * DNNID.h
 *
 *  Created on: 2023. 12. 25.
 *      Author: Suhwan
 */

#ifndef __DNN_DNNID_H
#define __DNN_DNNID_H

#include <iostream>
#include <string>
#include <chrono>
#include <omnetpp.h>

#include "ddc/libs/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace inet {

class DNNID
{
    private:
        string agent_name;
        string dnn_model_name;
        double input_size;
        double start_simulation_time;
        double dnn_identifier;

    public:
        DNNID(); 
        DNNID(string dnn_model_name, string agent_name, double input_size, double start_simulation_time, double dnn_identifier);

        string toString() const;
    
        string getAgentName() const { return this->agent_name; }
        string getDNNModelName() const { return this->dnn_model_name; }
        double getInputSize() const { return this->input_size; }
        double getDNNIdentifier() const { return this->dnn_identifier; }
        double getStartSimulationTime() const { return this->start_simulation_time; }

        bool operator==(const DNNID& other) const;

        string serialize() const;
        void deserialize(string json_string);
};

}

#endif // ifndef __DNN_DNNID_H
