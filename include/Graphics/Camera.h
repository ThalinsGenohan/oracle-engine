#ifndef ORACLES_CAMERA_H
#define ORACLES_CAMERA_H

#include <OtterML/Graphics/Transform.hpp>

#include "types.h"

namespace oter
{
class GameObject;

class Camera
{
public:
	explicit Camera(const v2u& size);

	const Transform& GetTransform() const;
	void             SetTransform(const Transform& newTransform);
	const m3f&       GetMatrix();

	const Angle& GetRotation() const;
	const v2f& GetZoom() const;
	const v2f& GetSkew() const;
	const v2x& GetPosition() const;

	void SetRotation(const Angle& newRotation);
	void SetZoom(const v2f& newZoom);
	void SetZoom(const f32& newZoomScalar);
	void SetSkew(const v2f& newSkew);
	void SetPosition(const v2x& newPosition);

	void Rotate(const Angle& rotationChange);
	void Zoom(const v2f& zoomChange);
	void Zoom(const f32& zoomChangeScalar);
	void Skew(const v2f& skewChange);
	void Move(const v2x& positionChange);

	const Transform& GetTarget() const;
	void             SetTarget(const Transform& newTarget);

	const Angle& GetTargetRotation() const;
	const v2f& GetTargetZoom() const;
	const v2f& GetTargetSkew() const;
	const v2x& GetTargetPosition() const;

	bool IsAtTarget() const;

	bool IsSpeedLimited() const;
	void SetSpeedLimited(bool speedLimited);

	GameObject* GetFollowTarget() const;
	void        SetFollowTarget(GameObject* followTarget);

	void Update();

private:
	const f32 _rotationSpeed = 1.f;
	const v2f _zoomSpeed     = v2f(0.1f, 0.1f);
	const v2f _skewSpeed     = v2f(0.1f, 0.1f);
	const v2x _moveSpeed     = v2x(i32x8(4), i32x8(4));

	v2i _size;

	v2x _fixedPosition = v2x(0.0_x);
	v2x _fixedTargetPosition = v2x(0.0_x);

	Transform _transform;
	Transform _target;

	bool _speedLimited = true;

	GameObject* _followTarget = nullptr;

	void UpdateRotation();
	void UpdateZoom();
	void UpdateSkew();
	void UpdatePosition();
};
}

#endif
