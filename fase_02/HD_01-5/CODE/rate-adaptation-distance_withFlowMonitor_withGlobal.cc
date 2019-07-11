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

// FLOW MONITOR
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("RateAdaptationDistance");

class NodeStatistics
{
public:
  NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas);

  void CheckStatistics (double time);

  void RxCallback (std::string path, Ptr<const Packet> packet, const Address &from);
  void SetPosition (Ptr<Node> node, Vector position);
  void AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime);
  Vector GetPosition (Ptr<Node> node);

  Gnuplot2dDataset GetDatafile ();

private:
  uint32_t m_bytesTotal;
  Gnuplot2dDataset m_output;
};

NodeStatistics::NodeStatistics (NetDeviceContainer aps, NetDeviceContainer stas)
{
  m_bytesTotal = 0;
}

void
NodeStatistics::RxCallback (std::string path, Ptr<const Packet> packet, const Address &from)
{
  m_bytesTotal += packet->GetSize ();
}

void
NodeStatistics::CheckStatistics (double time)
{

}

void
NodeStatistics::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
NodeStatistics::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void
NodeStatistics::AdvancePosition (Ptr<Node> node, int stepsSize, int stepsTime)
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / (1000000 * stepsTime));
  m_bytesTotal = 0;
  m_output.Add (pos.x, mbs);
  pos.x += stepsSize;
  SetPosition (node, pos);
  Simulator::Schedule (Seconds (stepsTime), &NodeStatistics::AdvancePosition, this, node, stepsSize, stepsTime);
}

Gnuplot2dDataset
NodeStatistics::GetDatafile ()
{
  return m_output;
}


void RateCallback (std::string path, uint64_t rate, Mac48Address dest)
{
  NS_LOG_INFO ((Simulator::Now ()).GetSeconds () << " " << dest << " Rate " <<  rate / 1000000.0);
}


static ns3::GlobalValue g_shortGuardInterval ("shortGuardIntervalG",
							 "Enable Short Guard Interval in all stations",
               				 ns3::BooleanValue (false),
               				 ns3::MakeBooleanChecker());

static ns3::GlobalValue g_staManager ("staManagerG",
							 "Rate Adaptation algorithm",
               				 ns3::StringValue ("ns3::AarfWifiManager"),
               				 ns3::MakeStringChecker());


int main (int argc, char *argv[])
{
  //uint32_t rtsThreshold = 65535;
  uint32_t rtsThreshold = 5000;
  //std::string staManager = "ns3::MinstrelHtWifiManager";
  //std::string staManager = "ns3::AarfWifiManager";
  //std::string apManager = "ns3::MinstrelHtWifiManager";
  std::string apManager = "ns3::AarfWifiManager";
  //std::string standard = "802.11n-5GHz";
  std::string standard = "802.11a";
  //std::string outputFileName = "minstrelHT";
  std::string outputFileName = "aarf";
  uint32_t BE_MaxAmpduSize = 65535;
  //bool shortGuardInterval = false;
  uint32_t chWidth = 20;
  int ap1_x = 0;
  int ap1_y = 0;
  int sta1_x = 5;
  int sta1_y = 0;
  int steps = 1;
  int stepsSize = 1;
  int stepsTime = 1;

  CommandLine cmd;
  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();
  
  //cmd.AddValue ("staManager", "PRC Manager of the STA", staManager);
  cmd.AddValue ("apManager", "PRC Manager of the AP", apManager);
  cmd.AddValue ("standard", "Wifi Phy Standard", standard);
  //cmd.AddValue ("shortGuardInterval", "Enable Short Guard Interval in all stations", shortGuardInterval);
  cmd.AddValue ("channelWidth", "Channel width of all the stations", chWidth);
  cmd.AddValue ("rtsThreshold", "RTS threshold", rtsThreshold);
  cmd.AddValue ("BE_MaxAmpduSize", "BE Mac A-MPDU size", BE_MaxAmpduSize);
  cmd.AddValue ("outputFileName", "Output filename", outputFileName);
  cmd.AddValue ("steps", "How many different distances to try", steps);
  cmd.AddValue ("stepsTime", "Time on each step", stepsTime);
  cmd.AddValue ("stepsSize", "Distance between steps", stepsSize);
  cmd.AddValue ("AP1_x", "Position of AP1 in x coordinate", ap1_x);
  cmd.AddValue ("AP1_y", "Position of AP1 in y coordinate", ap1_y);
  cmd.AddValue ("STA1_x", "Position of STA1 in x coordinate", sta1_x);
  cmd.AddValue ("STA1_y", "Position of STA1 in y coordinate", sta1_y);
  cmd.Parse (argc, argv);


  UintegerValue uintegerValue;
  IntegerValue integerValue;
  DoubleValue doubleValue;
  BooleanValue booleanValue;
  StringValue stringValue;


  GlobalValue::GetValueByName ("shortGuardIntervalG", booleanValue);
  uint32_t shortGuardInterval = booleanValue.Get ();
  GlobalValue::GetValueByName ("staManagerG", stringValue);
  std::string staManager = stringValue.Get ();

  int simuTime = steps * stepsTime;

  // Define the APs
  NodeContainer wifiApNodes;
  wifiApNodes.Create (1);

  //Define the STAs
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (1);


  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  wifiPhy.Set ("ShortGuardEnabled", BooleanValue (shortGuardInterval));

  NetDeviceContainer wifiApDevices;
  NetDeviceContainer wifiStaDevices;
  NetDeviceContainer wifiDevices;

  WifiHelper wifi;
  if (standard == "802.11a" || standard == "802.11b" || standard == "802.11g")
    {
      if (standard == "802.11a")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
        }
      else if (standard == "802.11b")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
        }
      else if (standard == "802.11g")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211g);
        }
      WifiMacHelper wifiMac;

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }
  else if (standard == "802.11n-2.4GHz" || standard == "802.11n-5GHz")
    {
      if (standard == "802.11n-2.4GHz")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
        }
      else if (standard == "802.11n-5GHz")
        {
          wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
        }

      WifiMacHelper wifiMac;

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }
  else if (standard == "802.11ac")
    {
      wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
      WifiMacHelper wifiMac;

      //Configure the STA node
      wifi.SetRemoteStationManager (staManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      Ssid ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiStaDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiStaNodes.Get (0)));

      //Configure the AP node
      wifi.SetRemoteStationManager (apManager, "RtsCtsThreshold", UintegerValue (rtsThreshold));

      ssid = Ssid ("AP");
      wifiMac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid),
                       "BE_MaxAmpduSize", UintegerValue (BE_MaxAmpduSize));
      wifiApDevices.Add (wifi.Install (wifiPhy, wifiMac, wifiApNodes.Get (0)));
    }

  wifiDevices.Add (wifiStaDevices);
  wifiDevices.Add (wifiApDevices);

  // Set channel width
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (chWidth));

  // Configure the mobility.
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  //Initial position of AP and STA
  positionAlloc->Add (Vector (ap1_x, ap1_y, 0.0));
  positionAlloc->Add (Vector (sta1_x, sta1_y, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNodes.Get (0));
  mobility.Install (wifiStaNodes.Get (0));

  //Statistics counter
  NodeStatistics atpCounter = NodeStatistics (wifiApDevices, wifiStaDevices);

  //Move the STA by stepsSize meters every stepsTime seconds
  Simulator::Schedule (Seconds (0.5 + stepsTime), &NodeStatistics::AdvancePosition, &atpCounter, wifiStaNodes.Get (0), stepsSize, stepsTime);

  //Configure the IP stack
  InternetStackHelper stack;
  stack.Install (wifiApNodes);
  stack.Install (wifiStaNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = address.Assign (wifiDevices);
  Ipv4Address sinkAddress = i.GetAddress (0);
  uint16_t port = 9;

  //Configure the CBR generator
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  ApplicationContainer apps_sink = sink.Install (wifiStaNodes.Get (0));

  OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (sinkAddress, port));
  onoff.SetConstantRate (DataRate ("400Mb/s"), 1420);
  onoff.SetAttribute ("StartTime", TimeValue (Seconds (0.5)));
  onoff.SetAttribute ("StopTime", TimeValue (Seconds (simuTime)));
  ApplicationContainer apps_source = onoff.Install (wifiApNodes.Get (0));

  apps_sink.Start (Seconds (0.5));
  apps_sink.Stop (Seconds (simuTime));

  //------------------------------------------------------------
  //-- Setup stats and data collection
  //--------------------------------------------

  //////////////////// Flow Monitor /////////////////////////
 AsciiTraceHelper asciiTraceHelper;
 Ptr<FlowMonitor> flowMonitor;
 FlowMonitorHelper flowHelper;
 flowMonitor = flowHelper.InstallAll();

 ////////////////////////////////////


 //Register packet receptions to calculate throughput
  Config::Connect ("/NodeList/1/ApplicationList/*/$ns3::PacketSink/Rx",
                   MakeCallback (&NodeStatistics::RxCallback, &atpCounter));

  //Callbacks to print every change of rate
  Config::Connect ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/RemoteStationManager/$" + apManager + "/RateChange",
                   MakeCallback (RateCallback));

  Simulator::Stop (Seconds (simuTime));
  Simulator::Run ();

static bool verbose = true;
uint32_t m_bytesTotal=0;
std::string dl_results,ul_results;
dl_results = "DL_Results_Sim_PropModel_.txt";
ul_results = "UL_Results_Sim_PropModel_.txt";


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
         std::cout << "sink address " << sinkAddress  <<std::endl;
         std::cout << "destination address " << t.destinationAddress  <<std::endl;
         if ((t.destinationPort == port)
             & (t.sourceAddress == i.GetAddress (1)))
           {
             streamMetricsInit = DLstreamMetricsInit;
           }
         // Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
         else if ((t.destinationPort == port)
             & (t.destinationAddress == sinkAddress))
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
  
  std::ofstream outfile (("throughput-" + outputFileName + ".plt").c_str ());
  Gnuplot gnuplot = Gnuplot (("throughput-" + outputFileName + ".eps").c_str (), "Throughput");
  gnuplot.SetTerminal ("post eps color enhanced");
  gnuplot.SetLegend ("Time (seconds)", "Throughput (Mb/s)");
  gnuplot.SetTitle ("Throughput (AP to STA) vs time");
  gnuplot.AddDataset (atpCounter.GetDatafile ());
  gnuplot.GenerateOutput (outfile);
  



  Simulator::Destroy ();

  return 0;
}
