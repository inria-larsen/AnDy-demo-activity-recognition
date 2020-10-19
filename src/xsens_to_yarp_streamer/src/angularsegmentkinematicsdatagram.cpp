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

#include "angularsegmentkinematicsdatagram.h"
#include <xsens/xsmath.h>
#include <iomanip>
//#include <math.h>

/*! \class AngularSegmentKinematicsDatagram
	\brief a Angular Kinematics datagram (type 0x22)

	Information about each segment is sent as follows.

	4 bytes segment ID, in the range 1-30
	4 bytes q1 segment orientation quaternion component 1 (re)
	4 bytes q2 segment orientation quaternion component 1 (i)
	4 bytes q3 segment orientation quaternion component 1 (j)
	4 bytes q4 segment orientation quaternion component 1 (k)
	4 bytes angular velocity around segment x axis
	4 bytes angular velocity around segment y axis
	4 bytes angular velocity around segment z axis
	4 bytes angular acceleration around segment x axis
	4 bytes angular acceleration around segment y axis
	4 bytes angular acceleration around segment z axis

	Total: 44 bytes per segment

	The coordinates use a Z-Up, right-handed coordinate system.
*/

/*! Constructor */
AngularSegmentKinematicsDatagram::AngularSegmentKinematicsDatagram(yarp::os::BufferedPort<yarp::os::Bottle> &port)
	: Datagram(port)
{
	setType(SPAngularSegmentKinematics);
}

/*! Destructor */
AngularSegmentKinematicsDatagram::~AngularSegmentKinematicsDatagram()
{
}

/*! Deserialize the data from \a arr
	\sa serializeData
*/
void AngularSegmentKinematicsDatagram::deserializeData(Streamer &inputStreamer)
{
	Streamer* streamer = &inputStreamer;

	for (int i = 0; i < dataCount(); i++)
	{
		Kinematics kin;

		// Store the segement Id -> 4 byte 
		streamer->read(kin.segmentId);

		// Store the Segment orientation in a Vector -> 16 byte	(4 x 4 byte)
		for (int k = 0; k < 4; k++)
			streamer->read(kin.segmentOrien[k]);
		// DEBUG SERE+ ADRIEN 14-09-2020 
		// we don't transform in deg because this does not make sense 
		// and it does not match the signal we use for training when we take this signal offline
		//
		//// trasform in degrees
		//for (int k = 0; k < 4; k++)
		//	kin.segmentOrien[k] = XsMath_rad2deg(kin.segmentOrien[k]);

		/* THIS IS USEFUL TO DEBUG THE CONVERTED VALUES
		if ((kin.segmentId == 1))
		{
			//std::cout << "DEBUG Sere: segment orientation " << kin.segmentOrien[0] << " " << kin.segmentOrien[1] << " "
			//	<< kin.segmentOrien[2] << " " << kin.segmentOrien[3] << " --- ";

			struct Quaternion {
				double w, x, y, z;
			};

			struct EulerAngles {
				double roll, pitch, yaw;
			};

			Quaternion q; 
			q.w = kin.segmentOrien[0]; 
			q.x = kin.segmentOrien[1];
			q.y = kin.segmentOrien[2];
			q.z = kin.segmentOrien[3];
			
			EulerAngles angles;


			// roll (x-axis rotation)
			double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			angles.roll = std::atan2(sinr_cosp, cosr_cosp);

			// pitch (y-axis rotation)
			double sinp = 2 * (q.w * q.y - q.z * q.x);
			if (std::abs(sinp) >= 1)
				//angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
				angles.pitch = std::copysign(XsMath_pi / 2, sinp); // use 90 degrees if out of range
			else
				angles.pitch = std::asin(sinp);

			// yaw (z-axis rotation)
			double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			angles.yaw = std::atan2(siny_cosp, cosy_cosp);

			std::cout << " DEBUG euler " << std::setw(20)<< XsMath_rad2deg(angles.roll) 
				<< " " << std::setw(20)<< XsMath_rad2deg(angles.pitch) 
				<< " " << std::setw(20)<< XsMath_rad2deg(angles.yaw) << std::endl;
			
		} 
		*/

		// Store the Angular Velocity in a Vector -> 12 byte	(3 x 4 byte)
		for (int k = 0; k < 3; k++)
			streamer->read(kin.angularVeloc[k]);
		// trasform in degrees
		for (int k = 0; k < 3; k++)
			kin.angularVeloc[k] = XsMath_rad2deg(kin.angularVeloc[k]);

		// Store the Angular Acceleration in a Vector -> 12 byte	(3 x 4 byte)
		for (int k = 0; k < 3; k++)
			streamer->read(kin.angularAccel[k]);
		// trasform in degrees
		for (int k = 0; k < 3; k++)
			kin.angularAccel[k] = XsMath_rad2deg(kin.angularAccel[k]);

		m_data.push_back(kin);
	}
}

/*! Print Data datagram in a formated why
*/
void AngularSegmentKinematicsDatagram::printData() const
{
	std::cout << "*********************** DATA CONTENT ***********************" <<  std::endl <<  std::endl;

	for (int i = 0; i < m_data.size(); i++)
	{
		std::cout << "Segment ID: " << m_data.at(i).segmentId << std::endl;
		// Segment orientation quaternion 
		std::cout << "Segment orientation: " << "(";
		std::cout << "re: " << m_data.at(i).segmentOrien[0] << ", ";
		std::cout << "i: " << m_data.at(i).segmentOrien[1] << ", ";
		std::cout << "j: " << m_data.at(i).segmentOrien[2] << ", ";
		std::cout << "k: " << m_data.at(i).segmentOrien[3] << ")"<< std::endl;

		// Angular Velocity
		std::cout << "Angular velocity: " << "(";
		std::cout << "x: " << m_data.at(i).angularVeloc[0] << ", ";
		std::cout << "y: " << m_data.at(i).angularVeloc[1] << ", ";
		std::cout << "z: " << m_data.at(i).angularVeloc[2] << ")"<< std::endl;

		// Angular acceleration
		std::cout << "Angular acceleration: " << "(";
		std::cout << "x: " << m_data.at(i).angularAccel[0] << ", ";
		std::cout << "y: " << m_data.at(i).angularAccel[1] << ", ";
		std::cout << "z: " << m_data.at(i).angularAccel[2] << ")"<< std::endl << std::endl;
	}
}

void AngularSegmentKinematicsDatagram::publishData()
{
  yarp::os::Bottle& bottle = port.prepare();
  bottle.clear();

  bottle.addInt(frameTime());
  
  for (int i = 0; i < m_data.size(); i++)
  {
    bottle.addInt(m_data.at(i).segmentId);
    // Segment orientation quaternion 
    bottle.addDouble(m_data.at(i).segmentOrien[0]);
    bottle.addDouble(m_data.at(i).segmentOrien[1]);
    bottle.addDouble(m_data.at(i).segmentOrien[2]);
    bottle.addDouble(m_data.at(i).segmentOrien[3]);

    // Angular Velocity
    bottle.addDouble(m_data.at(i).angularVeloc[0]);
    bottle.addDouble(m_data.at(i).angularVeloc[1]);
    bottle.addDouble(m_data.at(i).angularVeloc[2]);

    // Angular acceleration
    bottle.addDouble(m_data.at(i).angularAccel[0]);
    bottle.addDouble(m_data.at(i).angularAccel[1]);
    bottle.addDouble(m_data.at(i).angularAccel[2]);
  }
  port.write();
}
