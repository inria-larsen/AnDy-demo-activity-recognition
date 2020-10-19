/*! \file
	\section FileCopyright Copyright Notice
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "parsermanager.h"

#include "eulerdatagram.h"
#include "scaledatagram.h"
#include "metadatagram.h"
#include "quaterniondatagram.h"
#include "angularsegmentkinematicsdatagram.h"
#include "centerofmassdatagram.h"
#include "jointanglesdatagram.h"
#include "linearsegmentkinematicsdatagram.h"
#include "positiondatagram.h"
#include "timecodedatagram.h"
#include "trackerkinematicsdatagram.h"
#include "portsmap.h"

ParserManager::ParserManager(PortsMap &ports) : ports(ports)
{ 
}

/*! Destructor */
ParserManager::~ParserManager()
{
}

Datagram* ParserManager::createDgram(StreamingProtocol proto)
{
	switch (proto)
	{
	case SPPoseEuler:		return new EulerDatagram(ports[SPPoseEuler]);
	case SPPoseQuaternion:	return new QuaternionDatagram(ports[SPPoseQuaternion]);
	case SPPosePositions:	return new PositionDatagram(ports[SPPosePositions]);
	case SPMetaScaling:		return new ScaleDatagram(ports[SPMetaScaling]);
	case SPMetaMoreMeta:	return new MetaDatagram(ports[SPMetaMoreMeta]);
	case SPJointAngles:					return new JointAnglesDatagram(ports[SPJointAngles]);
	case SPLinearSegmentKinematics:		return new LinearSegmentKinematicsDatagram(ports[SPLinearSegmentKinematics]);
	case SPAngularSegmentKinematics:	return new AngularSegmentKinematicsDatagram(ports[SPAngularSegmentKinematics]);
	case SPTrackerKinematics:			return new TrackerKinematicsDatagram(ports[SPTrackerKinematics]);
	case SPCenterOfMass:				return new CenterOfMassDatagram(ports[SPCenterOfMass]);
	case SPTimeCode:					return new TimeCodeDatagram(ports[SPTimeCode]);

	default:
		return nullptr;
	}	
}

/*! Read single datagram from the incoming stream */
void ParserManager::readDatagram(const XsByteArray &data)
{
	StreamingProtocol type = static_cast<StreamingProtocol>(Datagram::messageType(data));
	Datagram *datagram = createDgram(type);

	if (datagram != nullptr) 
	{
		datagram->deserialize(data);

		//datagram->printHeader();
		//datagram->printData();
		datagram->publishData();
		delete datagram;
	}
}
