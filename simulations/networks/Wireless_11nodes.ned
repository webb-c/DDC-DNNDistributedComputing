//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

package simulations.networks;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.contract.INetworkNode;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

network Wireless_11nodes
{
    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=50,50");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,100");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=50,150");
        }
        node1: <default("AdhocHost")> like INetworkNode {
            @display("p=440,80");
        }
        node2: <default("AdhocHost")> like INetworkNode {
            @display("p=349,281");
        }
        node3: <default("AdhocHost")> like INetworkNode {
            @display("p=657,261");
        }
        node4: <default("AdhocHost")> like INetworkNode {
            @display("p=400,393");
        }
        node5: <default("AdhocHost")> like INetworkNode {
            @display("p=836,634");
        }
        node6: <default("AdhocHost")> like INetworkNode {
            @display("p=890,281");
        }
        node7: <default("AdhocHost")> like INetworkNode {
            @display("p=1098,478");
        }
        node8: <default("AdhocHost")> like INetworkNode {
            @display("p=1030,261");
        }
        node9: <default("AdhocHost")> like INetworkNode {
            @display("p=1015,208");
        }
        node10: <default("AdhocHost")> like INetworkNode {
            @display("p=1240,311");
        }
        node11: <default("AdhocHost")> like INetworkNode {
            @display("p=1273,186");
        }
        controlplane: <default("AdhocHost")> like INetworkNode {
            @display("p=183,89");
        }

}
