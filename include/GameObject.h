//
// Created by Thalins on 12/28/22.
//

#ifndef ORACLES_GAMEOBJECT_H
#define ORACLES_GAMEOBJECT_H

#include <string>
#include <vector>

#include <OtterML/Graphics/Transform.hpp>

#include "types.h"
#include "Graphics/Texture2D.h"

namespace oracle
{
class GameObject
{
public:
	explicit GameObject(Texture2D& texture);

	const Transform&         GetTransform() const;
	void                     SetTransform(const Transform& newTransform);
	const m3f& GetMatrix();

	const oter::Angle& GetRotation() const;
	const v2f& GetScale() const;
	const v2f& GetSkew() const;
	const v2x& GetPosition() const;

	void SetRotation(const oter::Angle& newRotation);
	void SetScale(const v2f& newScale);
	void SetScale(const f32& newScaleScalar);
	void SetSkew(const v2f& newSkew);
	void SetPosition(const v2x& newPosition);

	void Rotate(const oter::Angle& rotationChange);
	void Scale(const v2f& scaleChange);
	void Scale(const f32& scaleChangeScalar);
	void Skew(const v2f& skewChange);
	void Move(const v2x& positionChange);

	const Transform& GetTarget() const;
	void             SetTarget(const Transform& newTarget);

	const oter::Angle& GetTargetRotation() const;
	const v2f& GetTargetZoom() const;
	const v2f& GetTargetSkew() const;
	const v2x& GetTargetPosition() const;

	bool IsAtTarget() const;

	bool IsSpeedLimited() const;
	void SetSpeedLimited(bool speedLimited);

	void SetRotationSpeed(const f32& rotationSpeed);
	void SetScaleSpeed(const v2f& scaleSpeed);
	void SetSkewSpeed(const v2f& skewSpeed);
	void SetMoveSpeed(const v2x& moveSpeed);

	bool IsAnimationUpdatePending() const;
	void SetAnimationUpdatePending(bool update);

	const v2u& GetFramePosition() const;

	const std::string&                            GetCurrentAnimationID() const;
	u32                                           GetCurrentAnimationFrameNumber() const;
	const std::vector<Texture2D::AnimationFrame>& GetCurrentAnimation() const;
	const Texture2D::AnimationFrame&              GetCurrentAnimationFrame();
	void                                          PlayAnimation(const std::string& animation);

	const Texture2D& GetTexture() const;

	void Update(float deltaMs);

private:
	v2x _fixedPosition = v2x(0.0_x);
	v2x _fixedTargetPosition = v2x(0.0_x);

	bool _speedLimited  = false;
	f32  _rotationSpeed = 1.f;
	v2f  _scaleSpeed    = v2f(0.1f, 0.1f);
	v2f  _skewSpeed     = v2f(0.1f, 0.1f);
	v2x  _moveSpeed     = v2x(4.0_x, 4.0_x);

	Transform   _transform              = Transform();
	Transform   _target                 = Transform();
	bool        _animationUpdatePending = false;
	std::string _currentAnimationID     = "";
	u32         _currentFrameNumber     = 0;
	f32         _frameTimerMs           = 0.f;

	v2i _direction = v2i::Zero;

	Texture2D& _texture;

	// Vertex data
	v2u _framePosition = v2u::Zero;

	void UpdateRotation();
	void UpdateScale();
	void UpdateSkew();
	void UpdatePosition();
};
}

#endif // ORACLES_GAMEOBJECT_H
