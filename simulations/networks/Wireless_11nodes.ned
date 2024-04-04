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
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth1G;
import inet.node.ethernet.Eth10G;

network Wireless_11nodes
{
    types:
        channel Eth200M extends EthernetLink
        {
            datarate = 200Mbps;
        }
        channel Eth500M extends EthernetLink
        {
            datarate = 500Mbps;
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
        node1: StandardHost {
            @display("p=440,80");
        }
        node2: StandardHost {
            @display("p=349,281");
        }
        node3: StandardHost {
            @display("p=657,261");
        }
        node4: StandardHost {
            @display("p=400,393");
        }
        node5: StandardHost {
            @display("p=836,634");
        }
        node6: StandardHost {
            @display("p=890,281");
        }
        node7: StandardHost {
            @display("p=1098,478");
        }
        node8: StandardHost {
            @display("p=1030,261");
        }
        node9: StandardHost {
            @display("p=1015,208");
        }
        node10: StandardHost {
            @display("p=1240,311");
        }
        node11: StandardHost {
            @display("p=1273,186");
        }
        controlplane: StandardHost {
            @display("p=183,89");
        }
        
        connections:
        	node1.ethg++ <--> Eth100M <--> node2.ethg++;
        	node1.ethg++ <--> Eth100M <--> node3.ethg++;
        	
        	node2.ethg++ <--> Eth100M <--> node3.ethg++;
        	node2.ethg++ <--> Eth100M <--> node4.ethg++;
        	
        	node3.ethg++ <--> Eth100M <--> node6.ethg++;
        	
        	node4.ethg++ <--> Eth100M <--> node5.ethg++;
        	
        	node5.ethg++ <--> Eth100M <--> node7.ethg++;
        	node5.ethg++ <--> Eth100M <--> node6.ethg++;
        	
        	node6.ethg++ <--> Eth100M <--> node8.ethg++;
        	
        	node7.ethg++ <--> Eth100M <--> node8.ethg++;
        	node7.ethg++ <--> Eth100M <--> node10.ethg++; // node10[0].bitrate = 50
        	
        	node8.ethg++ <--> Eth100M <--> node9.ethg++; // node9[0].bitrate = 50
        	
        	node9.ethg++ <--> Eth100M <--> node11.ethg++; // node9[1].bitrate = 100
        	
        	node10.ethg++ <--> Eth100M <--> node11.ethg++; // node10[1].bitrate = 100
        	
			node1.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node2.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node3.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node4.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node5.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node6.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node7.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node8.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node9.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node10.ethg++ <--> Eth1G <--> controlplane.ethg++;
			node11.ethg++ <--> Eth1G <--> controlplane.ethg++;
        	

}
