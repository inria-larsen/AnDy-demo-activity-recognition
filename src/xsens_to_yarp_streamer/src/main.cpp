/*
 * Authors: Olivier Rochel, Adrien Malaise, Serena Ivaldi - INRIA
 */

#include <stdio.h>
#include <map>
#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/ResourceFinder.h>
#include <iostream>
#include <xstypes.h>
#include "datagram.h"
#include "udpserver.h"
#include "streamer.h"
#include "portsmap.h"
using namespace yarp::os;
PortsMap ports;

int main(int argc, char ** argv) {
    //Time::delay(1);

	
	
    //yarp initialization
    yarp::os::Network yarp;
    if (!yarp.checkNetwork()) {
        std::cerr << "Cannot connect to yarp network";
    }

	ResourceFinder rf;
	rf.setVerbose();
	rf.setDefaultContext("xsens-driver");
	rf.setDefaultConfigFile("xsens.ini"); 
	rf.configure(argc, argv);

	// These values should be overwritten if it finds the values in the config file
	std::string server_name = "192.168.137.1"; 
	//std::string server_name = "localhost";
	int server_port = 9763;

	std::cout << "\n\n";
	std::cout << " Reading params from file " << std::endl;

	

	if (rf.check("host"))
		server_name=rf.find("host").asString();
	if (rf.check("port"))
		server_port=rf.find("port").asInt();

	if (rf.check("IP_this_machine"))
	{
		server_name = rf.find("IP_this_machine").asString();
		std::cout << "Found param: IP_this_machine " << server_name << std::endl;
	}
	if (rf.check("server_port_xsens"))
	{
		server_port = rf.find("server_port_xsens").asInt();
		std::cout << "Found param: server_port_xsens " << server_port << std::endl;
	}

	ports[SPPoseEuler].open("/xsens/PoseEuler");
	//ports[SPPoseEuler].open("/xsens/PosePosition");
	ports[SPPoseQuaternion].open("/xsens/PoseQuaternion");
	ports[SPPosePositions].open("/xsens/PosePositions");
	ports[SPMetaScaling].open("/xsens/MetaScaling");
	ports[SPMetaMoreMeta].open("/xsens/MetaMoreMeta");
	ports[SPJointAngles].open("/xsens/JointAngles");
	ports[SPLinearSegmentKinematics].open("/xsens/LinearSegmentKinematics");
	ports[SPAngularSegmentKinematics].open("/xsens/AngularSegmentKinematics");
	ports[SPTrackerKinematics].open("/xsens/TrackerKinematics");
	ports[SPCenterOfMass].open("/xsens/COM");
	ports[SPTimeCode].open("/xsens/TimeCode");

	std::cout << " DEBUG SERE on yarp copy" << std::endl;

	// xsens initialization
	UdpServer udpServer(server_name, server_port, ports);

	// main loop
	while (true) {
		XsTime::msleep(100);
	}

    return 0;
}
