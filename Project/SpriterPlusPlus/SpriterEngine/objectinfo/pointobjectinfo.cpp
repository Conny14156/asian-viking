#include "pointobjectinfo.h"

namespace SpriterEngine
{
	PointObjectInfo::PointObjectInfo()
	{
	}

	point PointObjectInfo::getPosition() const
	{
		return position;
	}

	real PointObjectInfo::getAngle() const
	{
		return angle.angle;
	}

	void PointObjectInfo::setPosition(const point &newPosition)
	{
		position = newPosition;
	}

	void PointObjectInfo::setAngle(real newAngle)
	{
		angle.angle = newAngle;
	}

	void PointObjectInfo::setSpin(s_int newSpin)
	{
		angle.spinDirection = newSpin;
	}

	void PointObjectInfo::setObjectToLinear(UniversalObjectInterface *bObject, real t, UniversalObjectInterface *resultObject)
	{
		resultObject->setPosition(linear(position, bObject->getPosition(), t));
		resultObject->setAngle(angle.angleLinear(bObject->getAngle(), t));
		resultObject->setPosition(linear(position, bObject->getPosition(), t));
	}
}