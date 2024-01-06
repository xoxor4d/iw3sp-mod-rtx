#include "STDInc.hpp"

namespace RtxUtils
{
	void unit_quat_to_axis(const float* quat, float(*axis)[3])
	{
		float xx, xy, xz, xw;
		float yy, yz, yw;
		float zz, zw;

		const float scaledX = quat[0] + quat[0];
		xx = scaledX * quat[0];
		xy = scaledX * quat[1];
		xz = scaledX * quat[2];
		xw = scaledX * quat[3];

		const float scaledY = quat[1] + quat[1];
		yy = scaledY * quat[1];
		yz = scaledY * quat[2];
		yw = scaledY * quat[3];

		const float scaledZ = quat[2] + quat[2];
		zz = scaledZ * quat[2];
		zw = scaledZ * quat[3];

		(*axis)[0] = 1.0f - (yy + zz);
		(*axis)[1] = xy + zw;
		(*axis)[2] = xz - yw;
		(*axis)[3] = xy - zw;
		(*axis)[4] = 1.0f - (xx + zz);
		(*axis)[5] = yz + xw;
		(*axis)[6] = xz + yw;
		(*axis)[7] = yz - xw;
		(*axis)[8] = 1.0f - (xx + yy);
	}
}