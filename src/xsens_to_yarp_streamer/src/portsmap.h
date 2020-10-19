#pragma once
#include <yarp/os/BufferedPort.h>

typedef std::map<StreamingProtocol, yarp::os::BufferedPort<yarp::os::Bottle> > PortsMap;
