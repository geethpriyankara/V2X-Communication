#include "ns3/wave-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "ns3/netanim-module.h"
//#include "ns3/flow-monitor-module.h"

//Added for flow monitor
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include <string> 
#include <iostream>
#include <iomanip>

//std::string s = std::to_string(42);

//For colorful console printing
/*
 * Usage example :
 *    std::cout << BOLD_CODE << "some bold text << END_CODE << std::endl;
 *
 *    std::cout << YELLOW_CODE << BOLD_CODE << "some bold yellow text << END_CODE << std::endl;
 *
 */
#define YELLOW_CODE "\033[33m"
#define TEAL_CODE "\033[36m"
#define BOLD_CODE "\033[1m"
#define END_CODE "\033[0m"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WaveExample1");

Time VO_Delay;
Time VOPacketCount;
Time VOAveDelay;

Time VI_Delay;
Time VIPacketCount;
Time VIAveDelay;

Time BE_Delay;
Time BEPacketCount;
Time BEAveDelay;

Time BK_Delay;
Time BKPacketCount;
Time BKAveDelay;

//Time VO_txTime;

//float CollisionCount=0;
uint32_t TxCount=0;
uint32_t RxCount=0;

uint32_t numberOfNodes=50;


//Note: this is a promiscuous trace for all packet reception. This is also on physical layer, so packets still have WifiMacHeader
void Rx (std::string context, Ptr <const Packet> packet, uint16_t channelFreqMhz,  WifiTxVector txVector,MpduInfo aMpdu, SignalNoiseDbm signalNoise)
{
	//context will include info about the source of this event. Use string manipulation if you want to extract info.
	std::cout << BOLD_CODE <<  context << END_CODE << std::endl;
	//Print the info.
	std::cout << "\tSize=" << packet->GetSize()
			  << " Freq="<<channelFreqMhz
			  << " Mode=" << txVector.GetMode()
			  << " Signal=" << signalNoise.signal
			  << " Noise=" << signalNoise.noise << std::endl;
	RxCount=RxCount+1;
	std::cout << RxCount << std::endl;
	//We can also examine the WifiMacHeader
	WifiMacHeader hdr;
	if (packet->PeekHeader(hdr))
	{
		std::cout << "\tDestination MAC : " << hdr.GetAddr1() << "\tSource MAC : " << hdr.GetAddr2() << std::endl;
	}
}

/*
 * This function works for ns-3.30 onwards. For previous version, remove the last parameter (the "WifiPhyRxfailureReason")
 */
void RxDrop (std::string context, Ptr<const Packet> packet, ns3::WifiPhyRxfailureReason reason)
{
	std::cout << BOLD_CODE << YELLOW_CODE << "Packet Rx Dropped!" << END_CODE << std::endl;
	//From ns-3.30, the reasons are defined in an enum type in ns3::WifiPhy class.
	std::cout << " Reason : " << reason << std::endl;
	std::cout << context << std::endl;
	//char CollisionReason=char(reason);
	//if (reason.find(ns3::WifiPhyRxfailureReason("TXING")) != std::string::npos) {
	//CollisionCount=CollisionCount+1;
	//}
	//ns3::WifiPhyRxfailureReason test=ns3::WifiPhyRxfailureReason("TXING");
	//std::string test = std:: string(CollisionReason);
	std::string s = std::to_string(reason);
	std::cout << s << std::endl;
	//if (s=="7")
	//{
	//CollisionCount=CollisionCount+1;
	//}
	//std::cout << CollisionCount << std::endl;
	WifiMacHeader hdr;
	if (packet->PeekHeader(hdr))
		{

		std::cout << " Destination MAC : " << hdr.GetAddr1() << "\tSource MAC : " << hdr.GetAddr2() << "\tSeq No. " << hdr.GetSequenceNumber() << std::endl;
		}
	}

//Fired when a packet is Enqueued in MAC
void EnqueueTrace(std::string context, Ptr<const WifiMacQueueItem> item)
{
	std::cout << TEAL_CODE << "A Packet was enqueued : " << context << END_CODE << std::endl;

	Ptr <const Packet> p = item->GetPacket();
	/*
	 * Do something with the packet, like attach a tag. ns3 automatically attaches a timestamp for enqueued packets;
	 */

}
//Fired when a packet is Dequeued from MAC layer. A packet is dequeued before it is transmitted.
void DequeueTrace(std::string context, Ptr<const WifiMacQueueItem> item)
{
	std::cout << TEAL_CODE << "A Packet was dequeued : " << context << END_CODE << std::endl;

	//Ptr <const Packet> p = item->GetPacket();
	//Time queue_delay = Simulator::Now();
	//Time VO_txTime;
	
	if (context.find("VO_Txop") != std::string::npos) {
    	std::cout << "VO found!" << '\n';
    	Ptr <const Packet> p_VO = item->GetPacket();
    	Time VO_queue_delay = Simulator::Now()- item->GetTimeStamp();
    	VO_Delay=VO_Delay+VO_queue_delay;
    	std::cout << "VO Queuing delay=" << VO_Delay << std::endl;
    	VOPacketCount=VOPacketCount+Time(1);
    	std::cout << "VO Packet Count=" << VOPacketCount << std::endl;
    	VOAveDelay=Time(VO_Delay/VOPacketCount);
    	//TxCount=TxCount+1;
    	//VO_txTime== Simulator::Now();
    	//std::cout << "VO Packet dequeue time=" << VO_txTime << std::endl;
    	//std::cout << "VO Average Delay=" << VOAveDelay << std::endl;
    	}
    	
    	if (context.find("VI_Txop") != std::string::npos) {
    	std::cout << "VI found!" << '\n';
    	Ptr <const Packet> p_VI = item->GetPacket();
    	Time VI_queue_delay = Simulator::Now()- item->GetTimeStamp();
    	VI_Delay=VI_Delay+VI_queue_delay;
    	std::cout << "VI Queuing delay=" << VI_Delay << std::endl;
    	VIPacketCount=VIPacketCount+Time(1);
    	std::cout << "VI Packet Count=" << VIPacketCount << std::endl;
    	VIAveDelay=Time(VI_Delay/VIPacketCount);
    	//TxCount=TxCount+1;
    	//std::cout << "VI Average Delay=" << VIAveDelay << std::endl;
    	}
    	
    	if (context.find("BE_Txop") != std::string::npos) {
    	std::cout << "BE found!" << '\n';
    	Ptr <const Packet> p_BE = item->GetPacket();
    	Time BE_queue_delay = Simulator::Now()- item->GetTimeStamp();
    	BE_Delay=BE_Delay+BE_queue_delay;
    	std::cout << "BE Queuing delay=" << BE_Delay << std::endl;
    	BEPacketCount=BEPacketCount+Time(1);
    	std::cout << "BE Packet Count=" << BEPacketCount << std::endl;
    	BEAveDelay=Time(BE_Delay/BEPacketCount);
    	//TxCount=TxCount+1;
    	//std::cout << "BE Average Delay=" << BEAveDelay << std::endl;
    	}
    	
    	if (context.find("BK_Txop") != std::string::npos) {
    	std::cout << "BK found!" << '\n';
    	Ptr <const Packet> p_BK = item->GetPacket();
    	Time BK_queue_delay = Simulator::Now()- item->GetTimeStamp();
    	BK_Delay=BK_Delay+BK_queue_delay;
    	std::cout << "BK Queuing delay=" << BK_Delay << std::endl;
    	BKPacketCount=BKPacketCount+Time(1);
    	std::cout << "BK Packet Count=" << BKPacketCount << std::endl;
    	BKAveDelay=Time(BK_Delay/BKPacketCount);
    	//TxCount=TxCount+1;
    	//std::cout << "BK Average Delay=" << BKAveDelay << std::endl;
    	}
	
	//Keep in mind that a packet might get dequeued (dropped_ if it exceeded MaxDelay (default is 500ms)
	//std::cout << context+"Queuing delay=" << queue_delay << std::endl;


}



////////////////////////////////////////////// My Editing function ///////////////////////////////////////////////////////////

//double mean = (1/.01);
//double bound =rand();
//double value_sum_H = 0.0;
//double value_sum_D = 0.0;
//double value_sum_M = 0.0;
int T_H=100;
int T_D=500;
int T_C=100;
int T_M=0;
//int ZZ_Matrix[200000]={0};

int K_H=8;
int K_D=5;
int K_M=1;

double lambda_H=0.0001;
double lambda_D=0.0001;
double lambda_M=0.0001;

uint32_t simTime = 10000;

// function to generate and retrun expotential Event time.
uint32_t * generateExpEventtime(int K, int T, double lambda ) {
//double bound =0;
double mean = double(1.0/lambda);

   static uint32_t Add_Matrix [200]={0};
   Ptr<ExponentialRandomVariable> x_H = CreateObject<ExponentialRandomVariable> ();
   x_H->SetAttribute ("Mean", DoubleValue (mean));
   //x_H->SetAttribute ("Bound", DoubleValue (bound));
   
   double value_HH=0;
   //for (int i = 0; i < 4; ++i) {
    for(int j=0;j<200;j=j+K)
    {
       // if (i == 0)
       // {
                       
                double value_H = x_H->GetValue ();
                value_HH=value_HH+value_H;
                for (int m=0;m<K;m++)
                {
                Add_Matrix[j+m]=value_HH;
                value_HH = value_HH+T;
                }
    }

   return Add_Matrix;
}

// function to generate and retrun periodic Event time.
uint32_t * generatePeriodicEventtime(int T ) {
static uint32_t CAM_Matrix [200]={0};
int value_CC=T+(rand() % 10 +0);

 for(int j=0;j<200;j++)
    {
        CAM_Matrix[j]=value_CC;
        value_CC = value_CC+T;
    }
    
return CAM_Matrix;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{

////////////////////////////////////////////// My Editing main ///////////////////////////////////////////////////////////
    uint32_t *H;
    uint32_t *D;
    uint32_t *C;
    uint32_t *M;
    int x=0;
//uint32_t SentPackets=0;
//uint32_t Receivedpackets=0;
    uint32_t collisionCount=0;
   uint32_t channelUtilizeCount=0;

 
auto main_Array= new uint32_t[50][4][10000];

for(uint32_t i=0;i<numberOfNodes;i++) // node
{
	auto ZZ_HPD= new uint32_t[10000000];
	auto ZZ_DENM= new uint32_t[10000000];
	auto ZZ_CAM= new uint32_t[10000000];
	auto ZZ_MHD= new uint32_t[10000000];
    ZZ_HPD[10000000]={0};
    ZZ_DENM[10000000]={0};
    ZZ_CAM[10000000]={0};
    ZZ_MHD[10000000]={0};
        for(int j=0;j<4;j++) // packet type
        {
                if (j==0)
                        {
                                H=generateExpEventtime(K_H, T_H, lambda_H );  
                                //ZZ_HPD[200000]={0};
                                for (uint32_t l=0; l<200;l++)
                                   {
                                        x=H[l];
                                        ZZ_HPD[x]=1;
                                   
                                   } 
                                
                        }
                        if (j==1)
                        {
                                D=generateExpEventtime(K_D, T_D, lambda_D );  
                                //ZZ_DENM[200000]={0};
                                for (uint32_t l=0; l<200;l++)
                                   {
                                        x=D[l];
                                        ZZ_DENM[x]=1;
                                   
                                   } 
                        
                        }
                        if (j==2)
                        {
                        	C=generatePeriodicEventtime(T_C);
                                //ZZ_CAM[200000]={0};
                                for (uint32_t l=0; l<200;l++)
                                   {
                                        x=C[l];
                                        ZZ_CAM[x]=1;
                                   } 
                        
                        }
                        if (j==3)
                        {
                                M=generateExpEventtime(K_M, T_M, lambda_M );  
                                //ZZ_MHD[1000000]={0};
                                for (uint32_t l=0; l<200;l++)
                                   {
                                        x=M[l];
                                        ZZ_MHD[x]=1;
                                   
                                   } 
                        
                        }
                for(uint32_t t=0;t<simTime;t++) // time
                {
                        if (j==0)
                        {
                        main_Array[i][j][t]=round(ZZ_HPD[t]);
                        }
                        if (j==1)
                        {
                        main_Array[i][j][t]=round(ZZ_DENM[t]);
                        }
                        if (j==2)
                        {
                        main_Array[i][j][t]=round(ZZ_CAM[t]);
                        }
                        if (j==3)
                        {
                        main_Array[i][j][t]=round(ZZ_MHD[t]);
                        }
                
                        
                }
        }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






  CommandLine cmd;
  
  

  //Number of nodes
  uint32_t nNodes = uint32_t(numberOfNodes);

  cmd.AddValue ("n","Number of nodes", nNodes);

  cmd.Parse (argc, argv);

 
  NodeContainer nodes;
  nodes.Create(nNodes);

  //Nodes MUST have some sort of mobility because that's needed to compute the received signal strength
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  
  for (uint32_t i=0; i<nNodes;i++)
  {
  	 positionAlloc->Add (Vector ((rand() % 50 +0), (rand() % 50 +0), 0.0));
  }
  
  //positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  //positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  //positionAlloc->Add (Vector (5.0, 10.0, 0.0));
  //positionAlloc->Add (Vector (0.0, 10.0, 0.0));
  //positionAlloc->Add (Vector (5.0, 20.0, 0.0));

  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  //I prefer using WaveHelper to create WaveNetDevice
  YansWifiChannelHelper waveChannel = YansWifiChannelHelper::Default();
  YansWavePhyHelper wavePhy =  YansWavePhyHelper::Default();
  wavePhy.SetChannel (waveChannel.Create ());
  wavePhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);

  /*
   * If you create applications that control TxPower, define the low & high end of TxPower.
   * This is done by using 'TxInfo' as shown below.
   * 33 dBm is the highest allowed for non-government use (as per 802.11-2016 standard, page 3271
   * 44.8 dBm is for government use.
   *
   * Setting them to the same value is the easy way to go.
   * I can instead set TxPowerStart to a value lower than 33, but then I need to set the number of levels for each PHY
   */
  wavePhy.Set ("TxPowerStart", DoubleValue (8) );
  wavePhy.Set ("TxPowerEnd", DoubleValue (33) );



  QosWaveMacHelper waveMac = QosWaveMacHelper::Default ();
  WaveHelper waveHelper = WaveHelper::Default ();

  waveHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
  						"DataMode", StringValue ("OfdmRate6MbpsBW10MHz"	),
  						"ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),
  						"NonUnicastMode", StringValue ("OfdmRate6MbpsBW10MHz"));


  NetDeviceContainer devices = waveHelper.Install (wavePhy, waveMac, nodes);
  //wavePhy.EnablePcap ("WaveTest", devices);

  //prepare a packet with a payload of 500 Bytes. Basically it has zeros in the payload
 // Ptr <Packet> packet 	= Create <Packet> (1000);

  //destination MAC
  Mac48Address dest	= Mac48Address::GetBroadcast();

  /*
   * 0x88dc is the ethertype corresponding to WSMP. IPv4's etherType is 0x0800, and IPv6 is 0x86DD
   * The standard doesn't allow sending IP packets over CCH channel
   */
  uint16_t protocol = 0x88dc;

  //We can also set the transmission parameters at the higher layeres
  TxInfo tx;
  tx.preamble = WIFI_PREAMBLE_LONG;
  //We set the channel on which the packet is sent. The WaveNetDevice must have access to the channel
  //CCH is enabled by default.
  tx.channelNumber = CCH;

  //We can set per-packet data rate. This packet will have a rate of 12Mbps.
 // tx.dataRate = WifiMode ("OfdmRate12MbpsBW10MHz");

  /*
   * Set the Access Catogory (AC) of the packet.
   * The 802.11e EDCA standard defines 4 AC's named Voice, Video, Best Effort & Background in order of priority.
   * The value determines the EdcaQueue in which the packet will be enqueued.
   *
   * The 'tid' is a value from 0-7 that maps to ACs as follows
   * 1 or 2 : Background (Lowest priority)
   * 0 or 3 : Best effort
   * 4 or 5 : Video
   * 6 or 7 : Voice (Highest priority)
   */
//  tx.priority = 7;	//We set the AC to highest priority. We can set this per packet.

  /*
   * We can also set TxPower. This maps to the user define TxPowerStart & TxPowerEnd.
   * 7 : corresponds to the highest user-defined power (TxPowerEnd). In this code, it's 33 dBm
   * 1 : lowest (TxPowerStart). In this code, it's 8.3 dBm
   *
   * We'll have N equally spaced power levels.
   * A value of 8 indicates application don't want to set power or data rate. Values >8 are invalid.
   */
//  tx.txPowerLevel = 3; //When we define TxPowerStart & TxPowerEnd for a WifiPhy, 7 is correspond to TxPowerEnd, and 1 TxPowerStart, and numbers in between are levels.

  /*************** Sending a packet ***************/

  /*
   * In order to send a packet, we will call SendX function of WaveNetDevice.
   */

  //Get the WaveNetDevice for the first devices, using node 0.
//  Ptr <NetDevice> d0 = devices.Get (0);
 // Ptr <WaveNetDevice> wd0 = DynamicCast <WaveNetDevice> (d0);

  /*
   * We want to call
   *     wd0->SendX (packet, destination, protocol, tx);
   * By scheduling a simulator event as follows:
   */
 //  Simulator::Schedule ( Seconds (1) , &WaveNetDevice::SendX, wd0, packet, dest, protocol, tx);

  //Let us schedule try to have all three nodes schedule messages for broadcast
  for (uint32_t t=0 ; t<simTime; t++)
  {
	  //Go over all the nodes.
	  for (uint32_t i=0; i<devices.GetN() ; i++)
	  {
		  Ptr <NetDevice> di = devices.Get (i);
		  Ptr <WaveNetDevice> wdi = DynamicCast <WaveNetDevice> (di);

		  Ptr <Packet> packet_H = Create<Packet> (100);
		  Ptr <Packet> packet_D = Create<Packet> (100);
		  Ptr <Packet> packet_C = Create<Packet> (100);
		  Ptr <Packet> packet_M = Create<Packet> (100);

		  //Ptr <Packet> low_priority_packet = Create <Packet> (250); // A low priority packet

		  TxInfo txi;
      		  txi.preamble = WIFI_PREAMBLE_LONG;
		  txi.channelNumber = CCH;
		  //We always need to set TxPower to something. Otherwise, default data rate would be used.
		  txi.txPowerLevel = 7;
		  
		  
		  //for(uint32_t j=0;j<4;j++)
		  //{
			  if(main_Array[i][0][t]==1)
			  	{
			  	txi.dataRate = WifiMode ("OfdmRate6MbpsBW10MHz");
			 	txi.priority = 6; //check the pcap file for the TID value in the Wifi MAC header
			  	Simulator::Schedule ( MilliSeconds (t) , &WaveNetDevice::SendX, wdi, packet_H, dest, protocol, txi);
			  	 std::cout << "Node number=" << i << std::endl;
			  	  std::cout << "Time=" << t << std::endl;
			  	NS_LOG_UNCOND ("HPD packet Tx");
			   	TxCount=TxCount+1;
			  	}
			  	
			    if(main_Array[i][1][t]==1)
			  	{
			  	txi.dataRate = WifiMode ("OfdmRate6MbpsBW10MHz");
			  	txi.priority = 4; //check the pcap file for the TID value in the Wifi MAC header
			  	Simulator::Schedule ( MilliSeconds (t) , &WaveNetDevice::SendX, wdi, packet_D, dest, protocol, txi);
			  	std::cout << "Node number=" << i << std::endl;
			  	std::cout << "Time=" << t << std::endl;
			 	NS_LOG_UNCOND ("DENM packet Tx");
			  	TxCount=TxCount+1;
			  	}
			   if(main_Array[i][2][t]==1)
			  	{
			 // 	TxInfo tx;
  			//	tx.preamble = WIFI_PREAMBLE_LONG;
			 // 	tx.channelNumber = CCH;
			  	txi.dataRate = WifiMode ("OfdmRate6MbpsBW10MHz");
			  	txi.priority = 0; //check the pcap file for the TID value in the Wifi MAC header
			  //	 tx.txPowerLevel = 7;
			  	Simulator::Schedule ( MilliSeconds (t) , &WaveNetDevice::SendX, wdi, packet_C, dest, protocol, txi);
			  	std::cout << "Node number=" << i << std::endl;
			  	std::cout << "Time=" << t << std::endl;
			  	NS_LOG_UNCOND ("CAM packet Tx");
			  	TxCount=TxCount+1;
			  	}
			  if( main_Array[i][3][t]==1)
			  	{
			  	txi.dataRate = WifiMode ("OfdmRate6MbpsBW10MHz");
			  	txi.priority = 1; //check the pcap file for the TID value in the Wifi MAC header
			  	Simulator::Schedule ( MilliSeconds (t) , &WaveNetDevice::SendX, wdi, packet_M, dest, protocol, txi);
			  	std::cout << "Node number=" << i << std::endl;
			  	std::cout << "Time=" << t << std::endl;
			  	NS_LOG_UNCOND ("MHD packet Tx");
			  	TxCount=TxCount+1;
			  	}
		  //}
		  
		 // switch (i)
		 // {
		 // case 0:
		//	  //I am going to make node 0 broadcast at 27Mbps, with priority 5.
		//	  txi.dataRate = WifiMode ("OfdmRate27MbpsBW10MHz");
		//	  txi.priority = 5; //check the pcap file for the TID value in the Wifi MAC header
		//	  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
		//	  //We also are going to schedule another packet with lowest priority
		//	  txi.priority = 1; //1 is for BK (Background) priority.
		//	  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, low_priority_packet, dest, protocol, txi);

		//	  break;

		//  case 1:
			  //I am going to set only the data rate for packets sent by node1
		//	  txi.dataRate = WifiMode ("OfdmRate9MbpsBW10MHz");
		//	  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
		//	  break;

		//  case 2:
			  /* I am not going to set data rate for packets out of node 2.
			   * The data rate will be whatever we used for NonUnicastMode when we set WifiRemoteStationManager
			   */
		//	  Simulator::Schedule ( Seconds (t) , &WaveNetDevice::SendX, wdi, packet_i, dest, protocol, txi);
		//	  break;
		//  }
	  }
  }
  
  //std::cout << TxCount << std::endl;
  

  for(uint32_t t=0;t<simTime;t++) // time
  	{
  		uint32_t txoptCount=0;
  		//uint32_t channelUtilizeCount=0;
  	         NS_LOG_UNCOND ("New Time Tick");
  	        for(uint32_t i=0;i<numberOfNodes;i++) // node
  	        {
  	                 NS_LOG_UNCOND ("New Node");
  	                for(uint32_t j=0;j<4;j++) // packet type
  	               {
  	                    std::cout << "The value_sum of x is " << main_Array[i][j][t] << std::endl;
  	                    if(main_Array[i][j][t]==1)
  	                    {
  	                    	txoptCount=txoptCount+1;
  	                    }

  	                }
  	        }
  	        std::cout << "Tx opt count " << txoptCount << std::endl;
  	        if (txoptCount>=2)
  	        {
  	        	collisionCount=collisionCount+1;
  	        }
  	        if (txoptCount>=1)
  	        {
  	            channelUtilizeCount=channelUtilizeCount+1;
  	        }

  	}

  /****** Unicast Example *******/
  //Let's send a Unicast packet from n0 to n2
  //Get the MAC address of the target node
//  Ptr <WaveNetDevice> d2 = DynamicCast<WaveNetDevice>(devices.Get(2));
//  Mac48Address target_mac = Mac48Address::ConvertFrom (d2->GetAddress());

//  Ptr <Packet> unicast_packet = Create<Packet> (200);
 // TxInfo tx_u;
  /*
   * Schedule sending from WaveNetDevice 0.
   * Since this is a unicast, the frame will be acknowledged with an acknowledgment frame
   */
//  Simulator::Schedule ( Seconds(simTime-1) , &WaveNetDevice::SendX, wd0, unicast_packet, target_mac, protocol, tx_u );



  /* Using tracesources to trace some simulation events */

  /*
   * Connecting to a promiscous Rx trace source. This will invoke the 'Rx' function everytime a packet is received.
   *
   * The MonitorSnifferRx trace is defined in WifiPhy.
   */
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/MonitorSnifferRx", MakeCallback (&Rx) );

  //Set the number of power levels.
  Config::Set("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/TxPowerLevels", ns3::UintegerValue(7));


  /*
   * What if some packets were dropped due to collision, or whatever? We use this trace to fire RxDrop function
   */
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/PhyEntities/*/PhyRxDrop", MakeCallback (&RxDrop) );

  /*
   * We can also trace some MAC layer details
   */
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/MacEntities/*/$ns3::OcbWifiMac/*/Queue/Enqueue", MakeCallback (&EnqueueTrace));

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::WaveNetDevice/MacEntities/*/$ns3::OcbWifiMac/*/Queue/Dequeue", MakeCallback (&DequeueTrace));



  Simulator::Stop(MilliSeconds(simTime));

  Simulator::Run();

  Simulator::Destroy();
  std::cout << "VO Average Delay=" << (VOAveDelay) << std::endl;
  std::cout << "VI Average Delay=" << (VIAveDelay) << std::endl;
  std::cout << "BE Average Delay=" << (BEAveDelay) << std::endl;
  std::cout << "BK Average Delay=" << (BKAveDelay) << std::endl;
  std::cout << "No. of Transmitted Packets=" << TxCount << std::endl;
  std::cout << "No. of Received Packets=" << RxCount << std::endl;
  std::cout << "Number of collisions " << collisionCount << std::endl;
  std::cout << "Number of channel utilization " << channelUtilizeCount << std::endl;
  }
