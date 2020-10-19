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

#include "quaterniondatagram.h"
#include <xsens/xsmath.h>

/*! \class QuaternionDatagram
  \brief a Position & Quaternion orientation pose datagram (type 02)

  Information about each segment is sent as follows.

  4 bytes segment ID, in the range 1-30
  4 bytes x–coordinate of sensor position
  4 bytes y–coordinate of sensor position
  4 bytes z–coordinate of sensor position
  4 bytes q1 rotation – sensor rotation quaternion component 1 (re)
  4 bytes q2 rotation – sensor rotation quaternion component 1 (i)
  4 bytes q3 rotation – sensor rotation quaternion component 1 (j)
  4 bytes q4 rotation – sensor rotation quaternion component 1 (k)

  Total: 32 bytes per segment

  The coordinates use a Z-Up, right-handed coordinate system.
 */

/*! Constructor */
QuaternionDatagram::QuaternionDatagram(yarp::os::BufferedPort<yarp::os::Bottle> &port)
	: Datagram(port)
{
	setType(SPPoseQuaternion);
}

/*! Destructor */
QuaternionDatagram::~QuaternionDatagram()
{

}

/*! Deserialize the data from \a arr
	\sa serializeData
*/
void QuaternionDatagram::deserializeData(Streamer &inputStreamer)
{
	Streamer* streamer = &inputStreamer;

	for (int i = 0; i < dataCount(); i++)
	{
		Kinematics kin;

		// 4 byte
		streamer->read(kin.segmentId);

		// Store the Sensor Position in a Vector -> 12 byte	(3 x 4 byte)
		// The coordinates use a Z-Up, right-handed coordinate system.
		for (int k = 0; k < 3; k++)
			streamer->read(kin.sensorPos[k]);

		// Store the Quaternion Rotation in a vector -> 16 byte	(4 x 4 byte) 
		for (int k = 0; k < 4; k++)
			streamer->read(kin.quatRotation[k]);

		// trasform in degrees
		for (int k = 0; k < 4; k++)
			kin.quatRotation[k] = XsMath_rad2deg(kin.quatRotation[k]); // TODO SERE: why?? this must be removed. do it when it is debugged on the robot code as well.

		m_data.push_back(kin);
	}
}

/*! Print Data datagram in a formated why
*/
void QuaternionDatagram::printData() const
{
	std::cout << "*********************** DATA CONTENT ***********************" <<  std::endl <<  std::endl;
	for (int i = 0; i < m_data.size(); i++)
	{
		std::cout << "Segment ID: " << m_data.at(i).segmentId << std::endl;
		// Sensor Position
		std::cout << "Segment Position: " << "(";
		std::cout << "x: " << m_data.at(i).sensorPos[0] << ", ";
		std::cout << "y: " << m_data.at(i).sensorPos[1] << ", ";
		std::cout << "z: " << m_data.at(i).sensorPos[2] << ")"<< std::endl;

		// Quaternion Rotation
		std::cout << "Quaternion Rotation: " << "(";
		std::cout << "re: " << m_data.at(i).quatRotation[0] << ", ";
		std::cout << "i: " << m_data.at(i).quatRotation[1] << ", ";
		std::cout << "j: " << m_data.at(i).quatRotation[2] << ", ";
		std::cout << "k: " << m_data.at(i).quatRotation[3] << ")"<< std::endl << std::endl;
	}
}

void QuaternionDatagram::publishData()
{
  yarp::os::Bottle& bottle = port.prepare();
  bottle.clear();

  //bottle.addInt(frameTime());
  for (int i = 0; i < m_data.size(); i++)
  {
    //bottle.addInt(m_data.at(i).segmentId);
    bottle.addDouble(m_data.at(i).sensorPos[0]);
    bottle.addDouble(m_data.at(i).sensorPos[1]);
    bottle.addDouble(m_data.at(i).sensorPos[2]);
    
    bottle.addDouble(m_data.at(i).quatRotation[0]);
    bottle.addDouble(m_data.at(i).quatRotation[1]);
    bottle.addDouble(m_data.at(i).quatRotation[2]);
    bottle.addDouble(m_data.at(i).quatRotation[3]);

  }
  port.write();
}
