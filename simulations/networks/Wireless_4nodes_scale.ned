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

import inet.node.inet.StandardHost;
import inet.node.ethernet.EthernetLink;
import inet.node.ethernet.Eth1G;


network Wireless_4nodes_scale
{
    
    types:
        channel Eth10M extends EthernetLink
        {
            datarate = 10Mbps;
        }    
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
        // node1: <default("AdhocHost")> like INetworkNode {
        //    @display("p=453,280");
        // }
        node1: StandardHost {
            @display("p=453,280");
        }
        node2: StandardHost {
            @display("p=802,167");
        }
        node3: StandardHost {
            @display("p=774,391");
        }
        node4: StandardHost {
            @display("p=1202,222");
        }
        controlplane: StandardHost {
            @display("p=724,37");
        }

    connections allowunconnected:
        node1.ethg++ <--> Eth10M <--> node2.ethg++;
        node1.ethg++ <--> Eth10M <--> node3.ethg++;
        node2.ethg++ <--> Eth10M <--> node4.ethg++;
        node3.ethg++ <--> Eth10M <--> node4.ethg++;
        
        node1.ethg++ <--> Eth1G <--> controlplane.ethg++;
        node2.ethg++ <--> Eth1G <--> controlplane.ethg++;
        node3.ethg++ <--> Eth1G <--> controlplane.ethg++;
        node4.ethg++ <--> Eth1G <--> controlplane.ethg++;

}
