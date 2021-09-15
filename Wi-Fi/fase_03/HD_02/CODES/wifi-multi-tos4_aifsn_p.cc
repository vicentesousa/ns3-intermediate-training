/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sebastien Deronne <sebastien.deronne@gmail.com>
 */
#include <ns3/config-store-module.h>
#include "ns3/core-module.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"
#include <chrono>

// This is a simple example in order to show how to configure an IEEE 802.11n Wi-Fi network
// with multiple TOS. It outputs the aggregated UDP throughput, which depends on the number of
// stations, the HT MCS value (0 to 7), the channel width (20 or 40 MHz) and the guard interval
// (long or short). The user can also specify the distance between the access point and the
// stations (in meters), and can specify whether RTS/CTS is used or not.
using namespace std::chrono;

auto start = high_resolution_clock::now();

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiMultiTos");

static ns3::GlobalValue g_nstas ("nstasG",
		"Number of stas",
		ns3::UintegerValue (1),
		ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_datarate("datarateG",
		"Offered Rate",
		ns3::UintegerValue (10000000.0),
		//ns3::UintegerValue (400000000.0),
		ns3::MakeUintegerChecker<uint32_t>());
static ns3::GlobalValue g_simulationTime ("simulationTimeG",
		"Simulation Time",
		ns3::UintegerValue (1),
		ns3::MakeUintegerChecker<uint32_t>());
static ns3::GlobalValue g_uplink ("uplinkstatusG",
               "False - Downlink // True - Uplink",
                       ns3::BooleanValue (false),
                       ns3::MakeBooleanChecker());
int main (int argc, char *argv[])
{
	//RngSeedManager::SetRun (1481931699445491);
	uint8_t channelWidth = 160; //Definindo a largura de banda (MHz) utilizada
	bool useShortGuardInterval = false; // Uso do Short Guard Interval
	bool useRts = false;

	CommandLine cmd;
	cmd.Parse (argc,argv);

	ConfigStore inputConfig;
	inputConfig.ConfigureDefaults ();
	inputConfig.ConfigureAttributes ();

	UintegerValue uintegerValue;
	IntegerValue integerValue;
	DoubleValue doubleValue;
	BooleanValue booleanValue;
	StringValue stringValue;

	GlobalValue::GetValueByName ("nstasG", uintegerValue);
	uint32_t nWifi = uintegerValue.Get (); // Variável global: Número de STAs
	GlobalValue::GetValueByName ("simulationTimeG", uintegerValue);
	uint32_t simulationTime = uintegerValue.Get (); // Variável global: Tempo de simulação
	GlobalValue::GetValueByName ("datarateG", uintegerValue);
	uint32_t offeredrate = uintegerValue.Get (); // Variável global: Taxa oferecida
	GlobalValue::GetValueByName ("uplinkstatusG", booleanValue);
	bool uplinkstatus = booleanValue.Get (); // Variável global: Caso de simulação - Downlink ou Uplink

	// Criação dos nós
	// STAs
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nWifi);
	// AP
	NodeContainer wifiApNode;
	wifiApNode.Create (1);

	// Criação do Canal
	YansWifiChannelHelper channel;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel", "ReferenceLoss", DoubleValue (40.089)); // Perda de referência para 5 GHz
	YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
	phy.SetChannel (channel.Create ());

	// Criando um helper para a configuração da MAC
	WifiMacHelper mac;
	// Criando um helper para a configuração do Wi-Fi
	WifiHelper wifi;
	wifi.SetStandard (WIFI_PHY_STANDARD_80211ac); // Padrão 802.11 utilizado

//	 Configuração do algoritmo de adaptação de taxa
	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
			                     "DataMode", StringValue("VhtMcs9"),
			                     "ControlMode", StringValue("VhtMcs9"),
			                     "RtsCtsThreshold", UintegerValue (useRts ? 0 : 999999));
//	wifi.SetRemoteStationManager("ns3::MinstrelHtWifiManager");

	Ssid ssid = Ssid ("ns3-80211ac");

	//Configuração da MAC
	mac.SetType ("ns3::StaWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);

	mac.SetType ("ns3::ApWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true),
				 "EnableBeaconJitter", BooleanValue (false));

	NetDeviceContainer apDevice;
	apDevice = wifi.Install (phy, mac, wifiApNode);

	// BE_Txop
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_Txop/MinCw", UintegerValue (15));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_Txop/MaxCw", UintegerValue (1023));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_Txop/Aifsn", UintegerValue (6));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BE_Txop/TxopLimit", TimeValue (MicroSeconds (0)));
	// BK_Txop
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_Txop/MinCw", UintegerValue (15));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_Txop/MaxCw", UintegerValue (1023));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_Txop/Aifsn", UintegerValue (9));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/BK_Txop/TxopLimit", TimeValue (MicroSeconds (0)));
	// VI_Txop
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_Txop/MinCw", UintegerValue (15));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_Txop/MaxCw", UintegerValue (1023));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_Txop/Aifsn", UintegerValue (3));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VI_Txop/TxopLimit", TimeValue (MicroSeconds (0)));
	// VO_Txop
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_Txop/MinCw", UintegerValue (15));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_Txop/MaxCw", UintegerValue (1023));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_Txop/Aifsn", UintegerValue (2));
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/$ns3::RegularWifiMac/VO_Txop/TxopLimit", TimeValue (MicroSeconds (0)));

	// Set channel width
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (channelWidth));

	// Set guard interval
	Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/HtConfiguration/ShortGuardIntervalSupported", BooleanValue (useShortGuardInterval));

	// mobility
	MobilityHelper mobilityAP;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	positionAlloc->Add (Vector (0.0, 0.0, 0.0));
	mobilityAP.SetPositionAllocator (positionAlloc);
	mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobilityAP.Install (wifiApNode);

	MobilityHelper mobilitySTA;
	mobilitySTA.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobilitySTA.SetPositionAllocator("ns3::RandomRectanglePositionAllocator",
			"X", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0] "),
			"Y", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0]"));
	mobilitySTA.Install (wifiStaNodes);

	// Internet stack
	InternetStackHelper stack;
	stack.Install (wifiApNode);
	stack.Install (wifiStaNodes);
	Ipv4AddressHelper address;

	address.SetBase ("192.168.1.0", "255.255.255.0");
	Ipv4InterfaceContainer staNodeInterfaces, apNodeInterface;

	apNodeInterface = address.Assign (apDevice);
	staNodeInterfaces = address.Assign (staDevices);


	Ipv4Address sinkaddressUL = apNodeInterface.GetAddress(0);
	Ipv4InterfaceContainer sinkaddressDL;

	// Coletando os endereços de sink para o Downlink
	Ipv4InterfaceContainer::Iterator iteripv4;
	int k=0;
	for (iteripv4=staNodeInterfaces.Begin(); iteripv4 < staNodeInterfaces.End(); iteripv4++){
		sinkaddressDL.Add(staNodeInterfaces.Get(k));
		k++;
	}


	// Setting applications
	ApplicationContainer sourceApplications, sinkApplications;
	std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
	uint32_t portNumber = 5000;

	if (uplinkstatus == false){
		for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){

				InetSocketAddress sinkSocket (sinkaddressDL.GetAddress(i), portNumber++);
				sinkSocket.SetTos (tosValue);
				OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
				onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1000]"));
				onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
				onOffHelper.SetAttribute ("DataRate", DataRateValue (offeredrate));
				onOffHelper.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
				sourceApplications.Add (onOffHelper.Install (wifiApNode.Get (0)));
				PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
				sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes));
			}
		}
	}

	if (uplinkstatus == true){
		for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){

				InetSocketAddress sinkSocket (sinkaddressUL, portNumber++);
				sinkSocket.SetTos (tosValue);
				OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
				onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1000]"));
				onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));;
				onOffHelper.SetAttribute ("DataRate", DataRateValue (offeredrate));
				onOffHelper.SetAttribute ("PacketSize", UintegerValue (1472)); //bytes
				sourceApplications.Add (onOffHelper.Install (wifiStaNodes.Get(i)));
				PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
				sinkApplications.Add (packetSinkHelper.Install (wifiApNode.Get (0)));
			}
		}
	}

	sinkApplications.Start (Seconds (0.0));
	sinkApplications.Stop (Seconds (simulationTime + 1));
	sourceApplications.Start (Seconds (1.0));
	sourceApplications.Stop (Seconds (simulationTime + 1));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	//////////////////// Flow Monitor /////////////////////////
	AsciiTraceHelper asciiTraceHelper;
	Ptr<FlowMonitor> flowMonitor;
	FlowMonitorHelper flowHelper;
	flowMonitor = flowHelper.InstallAll();

	Simulator::Stop (Seconds (simulationTime + 1));
	Simulator::Run ();

	////////Flow Monitor data///////////////////////////////
	static bool verbose = true;
	uint32_t m_bytesTotal=0;
	std::string BE_results, BK_results, VO_results, VI_results;
	std::string offeredratetxt = std::to_string(offeredrate);
	std::string nstastxt = std::to_string(nWifi);
	if (uplinkstatus == false){
		BE_results = "DL_Results_BE_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		BK_results = "DL_Results_BK_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		VI_results = "DL_Results_VI_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		VO_results = "DL_Results_VO_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
	}
	if (uplinkstatus == true){
		BE_results = "UL_Results_BE_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		BK_results = "UL_Results_BK_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		VI_results = "UL_Results_VI_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
		VO_results = "UL_Results_VO_edca_" + nstastxt + "_" + offeredratetxt + ".txt";
	}


	Ptr<OutputStreamWrapper> BEstreamMetricsInit = asciiTraceHelper.CreateFileStream((BE_results));
	*BEstreamMetricsInit->GetStream()
            		  << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio"
					  << std::endl;

	Ptr<OutputStreamWrapper> BKstreamMetricsInit = asciiTraceHelper.CreateFileStream((BK_results));
	*BKstreamMetricsInit->GetStream()
            		  << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio"
					  << std::endl;

	Ptr<OutputStreamWrapper> VOstreamMetricsInit = asciiTraceHelper.CreateFileStream((VO_results));
	*VOstreamMetricsInit->GetStream()
            		  << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio"
					  << std::endl;

	Ptr<OutputStreamWrapper> VIstreamMetricsInit = asciiTraceHelper.CreateFileStream((VI_results));
	*VIstreamMetricsInit->GetStream()
            		  << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio"
					  << std::endl;

	double statDurationTX = 0;
	double statDurationRX = 0;
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter =
			stats.begin(); iter != stats.end(); ++iter)
	{
		// some metrics calculation
		statDurationRX = iter->second.timeLastRxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		statDurationTX = iter->second.timeLastTxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();

		double meanDelay, meanJitter, packetLossRatio, txTput, rxTput;
		if (iter->second.rxPackets > 0)
		{
			meanDelay = (iter->second.delaySum.GetSeconds()
					/ iter->second.rxPackets);
		}
		else // this value is set to zero because the STA is not receiving any packet
		{
			meanDelay = 0;
		}
		//
		if (iter->second.rxPackets > 1)
		{
			meanJitter = (iter->second.jitterSum.GetSeconds()
					/ (iter->second.rxPackets - 1));
		}
		else // this value is set to zero because the STA is not receiving any packet
		{
			meanJitter = 0;
		}
		//
		if (statDurationTX > 0)
		{
			txTput = iter->second.txBytes * 8.0 / statDurationTX / 1000 / 1000;
		}
		else
		{
			txTput = 0;
		}
		//
		if (statDurationRX > 0)
		{
			rxTput = iter->second.rxBytes * 8.0 / statDurationRX / 1000 / 1000;
		}
		else
		{
			rxTput = 0;
		}
		//
		if ((iter->second.lostPackets > 0) & (iter->second.rxPackets > 0))
		{
			packetLossRatio = (double) (iter->second.lostPackets
					/ (double) (iter->second.rxPackets + iter->second.lostPackets));
		}
		else
		{
			packetLossRatio = 0;
		}
		//
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
		//
		if (verbose)
		{
			// Print information only if enabled
			//
			//          std::cout << "Flow ID: " << iter->first << ", Source Port: "
			//              << t.sourcePort << ", Destination Port: " << t.destinationPort
			//              << " (" << t.sourceAddress << " -> " << t.destinationAddress
			//              << ")" << std::endl;
			//
			NS_LOG_UNCOND( "Flow ID: " << iter->first << ", Source Port: "
					<< t.sourcePort << ", Destination Port: " << t.destinationPort
					<< " (" << t.sourceAddress << " -> " << t.destinationAddress
					<< ")");
			//
			NS_LOG_UNCOND( "Lost Packets = " << iter->second.lostPackets);
			//
			NS_LOG_UNCOND( "Tx Packets = " << iter->second.txPackets);
			//
			NS_LOG_UNCOND( "Tx Bytes = " << iter->second.txBytes);
			//
			NS_LOG_UNCOND( "TxOffered = " << txTput << " Mbps");
			//std::cout << "TxOffered = " << txTput << " Mbps" << std::endl;
			//
			NS_LOG_UNCOND( "Rx Packets = " << iter->second.rxPackets);
			//
			NS_LOG_UNCOND( "Rx Bytes = " << iter->second.rxBytes);
			//
			NS_LOG_UNCOND( "T-put = " << rxTput << " Mbps");
			//std::cout << "T-put = " << rxTput << " Mbps" << std::endl;
			//
			NS_LOG_UNCOND( "Mean Delay Rx Packets = " << meanDelay << " s");
			//std::cout << "Mean Delay Rx Packets = " << meanDelay << " s"
			//    << std::endl;
			//
			NS_LOG_UNCOND( "Mean jitter = " << meanJitter << " s");
			//std::cout << "Mean jitter = " << meanJitter << " s" << std::endl;
			//
			NS_LOG_UNCOND( "Packet loss ratio = " << packetLossRatio);
			//std::cout << "Packet loss ratio = " << packetLossRatio << std::endl;
			//
		}
		//
		Ptr<OutputStreamWrapper> streamMetricsInit = NULL;
		// Get file pointer for DL, if DL flow (using port and IP address to assure correct result)
		std::cout << "t destination port " << t.destinationPort  <<std::endl;
		//std::cout << "source address " << interfaces.GetAddress(0)  <<std::endl;
		std::cout << "source address " << t.sourceAddress  <<std::endl;
		std::cout << "t destination port " << t.destinationPort  <<std::endl;
		//std::cout << "sink address " << sinkaddress  <<std::endl;
		std::cout << "destination address " << t.destinationAddress  <<std::endl;
//		if (uplinkstatus == true){
		for (uint32_t k = 5000; k < (5000+nWifi*4); k = k + 4){
			if ((t.destinationPort == k))
			{
				streamMetricsInit = BEstreamMetricsInit;
			}
			// Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
			else if ((t.destinationPort == k+1))
			{
				streamMetricsInit = BKstreamMetricsInit;
			}
			else if ((t.destinationPort == k+2))
			{
				streamMetricsInit = VIstreamMetricsInit;
			}
			else if ((t.destinationPort == k+3))
			{
				streamMetricsInit = VOstreamMetricsInit;
			}
		}
//		if (uplinkstatus == false){
//			if ((t.destinationAddress == staNodeInterfaces.GetAddress(0))
//					& (t.sourceAddress == apNodeInterface.GetAddress (0)) & (t.destinationPort == 5000))
//			{
//				streamMetricsInit = BEstreamMetricsInit;
//			}
//			// Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
//			else if ((t.destinationAddress == staNodeInterfaces.GetAddress(0))
//					& (t.sourceAddress == apNodeInterface.GetAddress (0)) & (t.destinationPort == 5001))
//			{
//				streamMetricsInit = BKstreamMetricsInit;
//			}
//			else if ((t.destinationAddress == staNodeInterfaces.GetAddress(0))
//					& (t.sourceAddress == apNodeInterface.GetAddress (0)) & (t.destinationPort == 5002))
//			{
//				streamMetricsInit = VIstreamMetricsInit;
//			}
//			else if ((t.destinationAddress == staNodeInterfaces.GetAddress(0))
//					& (t.sourceAddress == apNodeInterface.GetAddress (0)) & (t.destinationPort == 5003))
//			{
//				streamMetricsInit = VOstreamMetricsInit;
//			}
//		}
		//
		if (streamMetricsInit)
		{
			*streamMetricsInit->GetStream() << (iter->first ) << ", "
					<< (iter->second.lostPackets) << ", "
					//
					<< (iter->second.txPackets) << ", "
					//
					<< (iter->second.txBytes) << ", "
					//
					<< txTput << ", "
					//
					<< (iter->second.rxPackets) << ", "
					//
					<< (iter->second.rxBytes) << ", "
					//
					<< rxTput << ", "
					//
					<< meanDelay << ", "
					//
					<< meanJitter << ", "
					//
					<< packetLossRatio
					//
					<< std::endl;
		}
		else
		{
			//TODO: chance for an ASSERT
			if ( true )
			{
				std::cout << "Some problem to save metrics" << std::endl;
				std::cout << "Flow ID: " << iter->first << ", Source Port: "
						<< t.sourcePort << ", Destination Port: " << t.destinationPort
						<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
				std::cout << "AP Address: " << apNodeInterface.GetAddress (0) << std::endl;
				//std::cout << "DLport: " << port << std::endl;
				//std::cout << "ULport: " << port << std::endl;
			}
		}
		m_bytesTotal =+ iter->second.rxPackets;
	}

	flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);

	/////////////

	Simulator::Destroy ();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << duration.count() << std::endl;
	return 0;
}

