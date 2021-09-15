/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Universidad de la República - Uruguay
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
 * Author: Matías Richart <mrichart@fing.edu.uy>
 */

/**
 * This example program is designed to illustrate the behavior of
 * rate-adaptive WiFi rate controls such as Minstrel.  Power-adaptive
 * rate controls can be illustrated also, but separate examples exist for
 * highlighting the power adaptation.
 *
 * This simulation consist of 2 nodes, one AP and one STA.
 * The AP generates UDP traffic with a CBR of 54 Mbps to the STA.
 * The AP can use any power and rate control mechanism and the STA uses
 * only Minstrel rate control.
 * The STA can be configured to move away from (or towards to) the AP.
 * By default, the AP is at coordinate (0,0,0) and the STA starts at
 * coordinate (5,0,0) (meters) and moves away on the x axis by 1 meter every
 * second.
 *
 * The output consists of:
 * - A plot of average throughput vs. distance.
 * - (if logging is enabled) the changes of rate to standard output.
 *
 * Example usage:
 * ./waf --run "rate-adaptation-distance --staManager=ns3::MinstrelWifiManager --apManager=ns3::MinstrelWifiManager --outputFileName=minstrel"
 *
 * Another example (moving towards the AP):
 * ./waf --run "rate-adaptation-distance --staManager=ns3::MinstrelWifiManager --apManager=ns3::MinstrelWifiManager --outputFileName=minstrel --stepsSize=1 --STA1_x=-200"
 *
 * Example for HT rates with SGI and channel width of 40MHz:
 * ./waf --run "rate-adaptation-distance --staManager=ns3::MinstrelHtWifiManager --apManager=ns3::MinstrelHtWifiManager --outputFileName=minstrelHt --shortGuardInterval=true --channelWidth=40"
 *
 * To enable the log of rate changes:
 * export NS_LOG=RateAdaptationDistance=level_info
 */
#include "ns3/core-module.h"
#include <ns3/config-store-module.h>
#include "ns3/gnuplot.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/log.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/mobility-model.h"
#include "ns3/netanim-module.h"

// FLOW MONITOR
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("RateAdaptationDistance");


static ns3::GlobalValue g_nSTA ("nSTAG",
               "Number of Stations",
                       ns3::UintegerValue (10),
                       ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_staapmanager ("staapmanagerG",
               "Adaptation algorithm",
                       ns3::StringValue ("ns3::MinstrelHtWifiManager"),
                       ns3::MakeStringChecker());
static ns3::GlobalValue g_ratedown ("ratedownG",
               "Offered data Rate",
                       ns3::StringValue ("1.5Mbps"),
                       ns3::MakeStringChecker());
static ns3::GlobalValue g_rateup ("rateupG",
               "Offered data Rate",
                       ns3::StringValue ("750kbps"),
                       ns3::MakeStringChecker());
int main (int argc, char *argv[])
	{
  //uint32_t rtsThreshold = 65535;
  uint32_t rtsThreshold = 5000;
  //std::string apManager = "ns3::MinstrelHtWifiManager";
  //std::string staManager = "ns3::MinstrelHtWifiManager";
  std::string standard = "802.11ac";
  uint32_t BE_MaxAmpduSize = 65535;
  uint32_t simuTime = 500;
  uint32_t chWidth = 80;
  bool shortGuardInterval = false;
  


  CommandLine cmd;
  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();
  inputConfig.ConfigureAttributes ();


  UintegerValue uintegerValue;
  IntegerValue integerValue;
  DoubleValue doubleValue;
  BooleanValue booleanValue;
  StringValue stringValue;

  // new parameters
  GlobalValue::GetValueByName ("nSTAG", uintegerValue);
  uint32_t nSTA = uintegerValue.Get ();
  GlobalValue::GetValueByName ("ratedownG", stringValue);
  std::string ratedown = stringValue.Get ();
  GlobalValue::GetValueByName ("rateupG", stringValue);
  std::string rateup = stringValue.Get ();
  GlobalValue::GetValueByName ("staapmanagerG", stringValue);
  std::string apManager = stringValue.Get ();
  GlobalValue::GetValueByName ("staapmanagerG", stringValue);
    std::string staManager = stringValue.Get ();

  inputConfig.ConfigureAttributes ();

  // Define the APs
  NodeContainer wifiApNodes;
  wifiApNodes.Create (1);

  //Define the STAs
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nSTA);
  
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  wifiPhy.Set ("ShortGuardEnabled", BooleanValue (shortGuardInterval));

  NetDeviceContainer wifiApDevices;
  NetDeviceContainer wifiStaDevices;
  NetDeviceContainer wifiDevices;

  WifiHelper wifi;
  

      wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
      WifiMacHelper wifiMac;

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
   
  wifiDevices.Add (wifiApDevices);
  wifiDevices.Add (wifiStaDevices);
  

  // Set channel width
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (chWidth));

  // Configure the mobility of the STAs
  MobilityHelper mobility;
  MobilityHelper mobility1;
  MobilityHelper mobility2;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
	                                   "GridWidth", UintegerValue (3),
	                                   "MinX", DoubleValue (10),
	  								   "MinY", DoubleValue (10),
	  								   "DeltaX", DoubleValue(45.0),
	  								   "DeltaY", DoubleValue(45.0)); 
  mobility.Install(wifiStaNodes);
  if(nSTA>=5){
  mobility1.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                       "X", StringValue ("ns3::UniformRandomVariable[Min=33.0|Max=33.1]"),
                                       "Y", StringValue ("ns3::UniformRandomVariable[Min=33.0|Max=33.1]"));
  mobility1.Install(wifiStaNodes.Get(4));
  }if(nSTA==10){
  mobility2.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                         "X", StringValue ("ns3::UniformRandomVariable[Min=66.0|Max=66.1]"),
                                         "Y", StringValue ("ns3::UniformRandomVariable[Min=66.0|Max=66.1]"));
  mobility2.Install(wifiStaNodes.Get(9));
  }



// Configure the mobility of AP
  MobilityHelper mobilityap;
  mobilityap.SetPositionAllocator ("ns3::RandomRectanglePositionAllocator",
                                     "X", StringValue ("ns3::UniformRandomVariable[Min=50.0|Max=50.1]"),
                                     "Y", StringValue ("ns3::UniformRandomVariable[Min=50.1|Max=50.1]"));
  mobilityap.Install (wifiApNodes.Get (0));


  //Configure the IP stack
  InternetStackHelper stack;
  stack.Install (wifiStaNodes);
  stack.Install (wifiApNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  //Ipv4InterfaceContainer ista = address.Assign (wifiStaDevices);
  Ipv4InterfaceContainer i = address.Assign (wifiDevices);
  Ipv4Address sinkAddress = i.GetAddress (0);
  Ipv4InterfaceContainer ista;
  uint16_t port = 9;

   
Ipv4InterfaceContainer::Iterator iteripv4;
int k=0;
for (iteripv4 = i.Begin(); iteripv4 < i.End(); iteripv4++){

  
   if (k>0){

   ista.Add(i.Get(k)); 

}

k=k+1;
}

  
  

  //downlink
  ApplicationContainer apps_sink; 
  ApplicationContainer apps_source;
k=0;
for (iteripv4 = ista.Begin(); iteripv4 < ista.End(); iteripv4++){
	    std::cout << "iniciando " <<ista.GetAddress(k) << std::endl;
        OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (ista.GetAddress(k), port));
        onoff.SetConstantRate (DataRate (ratedown), 1420);
        onoff.SetAttribute ("StartTime", TimeValue (Seconds (0.5)));
        onoff.SetAttribute ("StopTime", TimeValue (Seconds (simuTime)));
        apps_source.Add(onoff.Install (wifiApNodes));

        PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (ista.GetAddress(k), port));
        apps_source.Add(sink.Install (wifiStaNodes.Get(k)));
        
        k=k+1;

       
}
  //uplink
  OnOffHelper onoff1 ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  onoff1.SetConstantRate (DataRate (rateup), 1420);
  onoff1.SetAttribute ("StartTime", TimeValue (Seconds (0.5)));
  onoff1.SetAttribute ("StopTime", TimeValue (Seconds (simuTime)));
  ApplicationContainer apps_source1 = onoff1.Install (wifiStaNodes);

  PacketSinkHelper sink1 ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  ApplicationContainer apps_sink1 = sink1.Install (wifiApNodes);

  

  //------------------------------------------------------------
  //-- Setup stats and data collection
  //--------------------------------------------

  //////////////////// Flow Monitor /////////////////////////
 AsciiTraceHelper asciiTraceHelper;
 Ptr<FlowMonitor> flowMonitor;
 FlowMonitorHelper flowHelper;
 flowMonitor = flowHelper.InstallAll();

 ////////////////////////////////////
 //std::string nSTAtxt1 = std::to_string(nSTA);
 //AnimationInterface anim("Anim_" + nSTAtxt1 + ".xml");
  AnimationInterface anim("Anim_Grid.xml");
  Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();

static bool verbose = true;
uint32_t m_bytesTotal=0;
std::string dl_results,ul_results;
std::string nSTAtxt = std::to_string(nSTA);
dl_results = "DL_Results_Sim_" + nSTAtxt + "_" + staManager + ".txt";
ul_results = "UL_Results_Sim_" + nSTAtxt + "_" + staManager + ".txt";


Ptr<OutputStreamWrapper> DLstreamMetricsInit = asciiTraceHelper.CreateFileStream((dl_results));
*DLstreamMetricsInit->GetStream()
            << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio"
            << std::endl;

Ptr<OutputStreamWrapper> ULstreamMetricsInit = asciiTraceHelper.CreateFileStream((ul_results));
*ULstreamMetricsInit->GetStream()
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
         //std::cout << "sink address " << sinkAddress  <<std::endl;
         std::cout << "destination address " << t.destinationAddress  <<std::endl;
         if ((t.destinationPort == port)
             & (t.sourceAddress == i.GetAddress (0)))
           {
             streamMetricsInit = DLstreamMetricsInit;
           }
         // Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
         else if ((t.destinationPort == port))
             //& (t.destinationAddress == sinkAddress))
           {
             streamMetricsInit = ULstreamMetricsInit;
           }
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
                 std::cout << "AP Address: " << i.GetAddress(0) << std::endl;
                 std::cout << "DLport: " << port << std::endl;
                 std::cout << "ULport: " << port << std::endl;
               }
           }
         m_bytesTotal =+ iter->second.rxPackets;
  }

 flowMonitor->SerializeToXmlFile("NameOfFile.xml", true, true);
  
  Simulator::Destroy ();

  return 0;
}
