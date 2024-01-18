/*
 * NodeInformation.cc
 *
 *  Created on: 2024. 01. 08.
 *      Author: Suhwan
 */

#include "ddc/applications/NodeInformation.h"

namespace inet
{   
    NodeInformation::NodeInformation() : NodeInformation::NodeInformation("", map<LayerNodePair, double>()){
    }

    NodeInformation::NodeInformation(string node_name, map<LayerNodePair, double> links_information){
        this->node_name = node_name;
        this->links_information = links_information;
    }

    string NodeInformation::serialize() const {
        json j;

        j["node_name"] = this->node_name;
        j["links_information"] = serializeLinksInformation();

        return j.dump();
    }

    void NodeInformation::deserialize(string json_string) {
        json j = json::parse(json_string);

        this->node_name = j["node_name"];
        deserializeLinksInformation(j["links_information"]);
    }

    string NodeInformation::serializeLinksInformation() const {
        json j;

        for (const auto &e : this->links_information) {
            j[e.first.serialize()] = to_string(e.second);
        }

        return j.dump();
    }

    void NodeInformation::deserializeLinksInformation(string json_string) {
        json j = json::parse(json_string);

        for (const auto& e : j.items()) {
            LayerNodePair link;
            link.deserialize(e.key());

            double queue_length = stof(string(e.value()));

            this->links_information[link] = queue_length;
        }
    }

} // namespace dnn
