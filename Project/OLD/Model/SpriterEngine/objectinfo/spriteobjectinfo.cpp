#include "spriteobjectinfo.h"

#include "../objectref/objectrefinstance.h"

namespace SpriterEngine
{
	SpriteObjectInfo::SpriteObjectInfo() :
		scale(1, 1),
		alpha(1)
	{
	}

	point SpriteObjectInfo::getPosition() const
	{
		return position;
	}

	real SpriteObjectInfo::getAngle() const
	{
		return angle.angle;
	}

	point SpriteObjectInfo::getScale() const
	{
		return scale;
	}

	real SpriteObjectInfo::getAlpha() const
	{
		return alpha;
	}

	point SpriteObjectInfo::getPivot() const
	{
		return pivot;
	}

	ImageFile *SpriteObjectInfo::getImage()
	{
		return imageFile;
	}

	void SpriteObjectInfo::setPosition(const point &newPosition)
	{
		position = newPosition;
	}

	void SpriteObjectInfo::setAngle(real newAngle)
	{
		angle.angle = newAngle;
	}

	void SpriteObjectInfo::setScale(const point &newScale)
	{
		scale = newScale;
	}

	void SpriteObjectInfo::setAlpha(real newAlpha)
	{
		alpha = newAlpha;
	}

	void SpriteObjectInfo::setPivot(const point &newPivot)
	{
		pivot = newPivot;
	}

	void SpriteObjectInfo::setImage(ImageFile *newImageFile)
	{
		imageFile = newImageFile;
	}

	void SpriteObjectInfo::setSpin(s_int newSpin)
	{
		angle.spinDirection = newSpin;
	}

	void SpriteObjectInfo::setToBlendedLinear(UniversalObjectInterface *aObject, UniversalObjectInterface *bObject, real t, real blendRatio, ObjectRefInstance *blendedRefInstance)
	{
		real tempAngle = angle.angle;
		point tempPosition = position;
		point tempScale = scale;
		real tempAlpha = alpha;
		point tempPivot = pivot;
		ImageFile *tempImageFile = imageFile;

		blendedRefInstance->preProcess();

		aObject->setObjectToLinear(bObject, t, this);

		setAngle(shortestAngleLinear(tempAngle, angle.angle, blendRatio));
		setPosition(linear(tempPosition, position, blendRatio));
		setScale(linear(tempScale, scale, blendRatio));
		setAlpha(linear(tempAlpha, alpha, blendRatio));

		if (blendRatio < 0.5)
		{
			setPivot(tempPivot);
			setImage(tempImageFile);
		}
	}

	void SpriteObjectInfo::render()
	{
		if (imageFile)
		{
			imageFile->renderSprite(this);
		}
	}
	void SpriteObjectInfo::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (imageFile)
		{
			imageFile->preRenderSprite(this);
			target.draw(*imageFile, states);
		}
	}
}