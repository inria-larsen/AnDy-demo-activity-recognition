# Copyright: (C) 2009 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# Make sure that YARP_DIR is set in your environment to wherever you 
# compiled YARP.

cmake_minimum_required(VERSION 2.8.9)

PROJECT(xsens)

FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_DATA_DIRS})

find_path(XSENS_INCLUDE_DIR "xstypes.h"
          HINTS "C:/Program Files/Xsens/MVN Studio Developer Toolkit 1.0.2/x64/include")

find_library(XSENS_LIBRARY NAMES xstypes64
             HINTS "C:/Program Files/Xsens/MVN Studio Developer Toolkit 1.0.2/x64/lib")
message(${XSENS_INCLUDE_DIR})
message(${XSENS_LIBRARY})

ADD_EXECUTABLE(xsens src/main.cpp src/udpserver.cpp src/streamer.cpp src/parsermanager.cpp src/datagram.cpp 
src/angularsegmentkinematicsdatagram.cpp src/centerofmassdatagram.cpp src/datagram.cpp src/eulerdatagram.cpp
src/jointanglesdatagram.cpp src/linearsegmentkinematicsdatagram.cpp src/metadatagram.cpp
src/positiondatagram.cpp src/quaterniondatagram.cpp src/scaledatagram.cpp src/streamer.cpp src/timecodedatagram.cpp
src/trackerkinematicsdatagram.cpp)
TARGET_LINK_LIBRARIES(xsens ${YARP_LIBRARIES})
include_directories(${XSENS_INCLUDE_DIR})
target_link_libraries(xsens ${XSENS_LIBRARY})
