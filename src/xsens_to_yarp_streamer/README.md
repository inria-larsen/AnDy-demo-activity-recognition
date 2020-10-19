# yarp "driver" for xsens

A very basic YARP driver to stream the Xsens datagrams on YARP ports.


## Requirements
* Xsens MVN 2018, which means it is installed in Windows

## Installation
* create a build directory
* run cmake-gui
* choose a compiler (visual 2015, x64 should work)
* configure
* generate
* open in visual studio, build the solution
* execute the file in build/release

## Usage
* Open Xsens MVN 2018 in Windows
* Launch a yarpserver somewhere (otherwise there is no YARP network)
* run xsens.exe (the one in build/release)

To check that it works, you can open a yarp port to read the content of the ports created by this module, which have the form: /xsens/PORT_NAME

## tested on
* windows x64 (visual c++ 2015 and visual studio 2019) - precisely Windows 10

## dependencies: 
xsens mvn sdk (xstypes.dll required, copy to build directory otherwise must be added to the system path)


