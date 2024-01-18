/*
 * Dijkstra.h
 *
 *  Created on: 2023. 12. 30.
 *      Author: Suhwan
 */

#include "ddc/layeredgraph/algorithm/Dijkstra.h"

namespace inet
{
    Dijkstra::Dijkstra() {}

    vector<LayerNode> Dijkstra::findPath(const map<LayerNodePair, double>& layered_graph,
                                         const map<LayerNode, vector<LayerNode>>& adjacency_matrix,
                                         const vector<LayerNode> layer_nodes,
                                         const LayerNode& src_layer_node,
                                         const LayerNode& dst_layer_node) {

        using NodeDistancePair = pair<double, LayerNode>;
        map<LayerNode, double> distances;
        map<LayerNode, LayerNode> previous;
        priority_queue<NodeDistancePair, vector<NodeDistancePair>, greater<NodeDistancePair>> priority_queue;

        initializeDistances(layer_nodes, distances, src_layer_node);
        priority_queue.push({0, src_layer_node});

        while (!priority_queue.empty()) {
            double current_distance = priority_queue.top().first;
            LayerNode current_node = priority_queue.top().second;

            priority_queue.pop();

            for (const LayerNode& neighbor : adjacency_matrix.at(current_node)) {
                double alt = current_distance + getEdgeWeight(layered_graph, current_node, neighbor);
                if (alt < distances[neighbor]) {
                    distances[neighbor] = alt;
                    previous[neighbor] = current_node;
                    priority_queue.push({alt, neighbor});
                }
            }
        }


        if (previous.size() == 0) {
            throw runtime_error("previous is empty.");
        }
    
        return reconstructPath(previous, src_layer_node, dst_layer_node);
    }

    void Dijkstra::initializeDistances(const vector<LayerNode>& layer_nodes,
                                       map<LayerNode, double>& distances,
                                       const LayerNode& src_layer_node) {
        for (const auto& layer_node : layer_nodes) {
            distances[layer_node] = numeric_limits<double>::infinity();
        }
        distances[src_layer_node] = 0;
    }

    double Dijkstra::getEdgeWeight(const map<LayerNodePair, double>& layered_graph,
                                   const LayerNode& src,
                                   const LayerNode& dst) {

        LayerNodePair link = LayerNodePair(src, dst);
        if (layered_graph.count(link) == 0) {
            return std::numeric_limits<double>::infinity(); // Return infinity if the link not found
        }
        else {
            return layered_graph.at(link);
        }


    }

    std::vector<LayerNode> Dijkstra::reconstructPath(const std::map<LayerNode, LayerNode>& previous,
                                                     const LayerNode& src_layer_node,
                                                     const LayerNode& dst_layer_node) {
        vector<LayerNode> path;
        if (previous.count(dst_layer_node) == 0) {
            string path_string = "";
            for (const auto &ln : previous) {
                path_string += ln.first.toString() + ln.second.toString() + "\n";
            }
            throw runtime_error(path_string);
        }
        for (LayerNode at = dst_layer_node; at != src_layer_node; at = previous.at(at)) {
            path.push_back(at);
        }
        path.push_back(src_layer_node);
        std::reverse(path.begin(), path.end());
        return path;
    }

}
