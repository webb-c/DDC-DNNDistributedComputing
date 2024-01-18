/*
 * NodeInformation.h
 *
 *  Created on: 2024. 01. 08.
 *      Author: Suhwan
 */

#ifndef __DDC_NODEINFORMATION_H
#define __DDC_NODEINFORMATION_H

#include <iostream>
#include <cstdint>

#include "ddc/libs/nlohmann/json.hpp"

#include "ddc/layernode/LayerNodePair.h"

using namespace std;
using json = nlohmann::json;

namespace inet {

class NodeInformation
{
    private:
        string node_name;
        map<LayerNodePair, double> links_information;


    public:
        NodeInformation();
        NodeInformation(string node_name, map<LayerNodePair, double> links_information);

        string getNodeName() { return this->node_name; }
        map<LayerNodePair, double> getLinksInformation() { return this->links_information; }

        string serialize() const;
        void deserialize(string json_string);

        string serializeLinksInformation() const;
        void deserializeLinksInformation(string json_string);
};

}

#endif // ifndef __DDC_NODEINFORMATION_H
