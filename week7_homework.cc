#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

using namespace ns3;


int
main(int argc, char *argv[]){

    std::string Delay = "10us", DataRate = "10Mbps";
    CommandLine cmd;

    cmd.AddValue("DataRate", "DataRatee", DataRate);
    cmd.AddValue("Delay", "Delayy", Delay);

    cmd.Parse(argc, argv);


    /* nodeContainer ~ */
    NodeContainer nodes;
    nodes.Create(2);

    /* p2p ~   */
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(DataRate));
    pointToPoint.SetChannelAttribute("Delay", StringValue(Delay));

    /* netDevice ~ */
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    /* internetStackhelper ~ */
    InternetStackHelper stack;
    stack.Install(nodes);


    /* ipv4addresshelper ~ */
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    /* udpechoClientHelper ~ */
    UdpClientHelper Client(interfaces.GetAddress(1), 9);
    Client.SetAttribute("MaxPackets", UintegerValue(10000));
    Client.SetAttribute("Interval", TimeValue(Seconds(0.001)));
    Client.SetAttribute("PacketSize", UintegerValue(1024));

    /* ApplicationContainer ~*/
    ApplicationContainer clientApps;
    clientApps.Add(Client.Install(nodes.Get(0)));
    clientApps.Start(Seconds(1.0));
    clientApps.Stop(Seconds(10.0));

    /* udpechoServerHelper ~ */
    UdpServerHelper Server(9);
    ApplicationContainer serverApps(Server.Install(nodes.Get(1)));
    serverApps.Start(Seconds(0.5));
    serverApps.Stop(Seconds(10.5));


    pointToPoint.EnablePcapAll("2017312108");
    Simulator::Run();
    Simulator::Stop(Seconds(11.0));

    Simulator::Destroy();
    return 0;
}
