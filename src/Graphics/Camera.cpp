#include "Graphics/Camera.h"

namespace oracle
{
Camera::Camera(const v2u& size) : _size(size) {}

const Transform& Camera::GetTransform() const { return this->_transform; }
void             Camera::SetTransform(const Transform& newTransform) { this->_transform = newTransform; }
const m3f&       Camera::GetMatrix() { return this->_transform.GetMatrix(); }

const Angle& Camera::GetRotation() const { return this->_transform.GetRotation(); }
const v2f& Camera::GetZoom() const { return this->_transform.GetScale(); }
const v2f& Camera::GetSkew() const { return this->_transform.GetSkew(); }
const v2x& Camera::GetPosition() const { return this->_fixedPosition; }

void Camera::SetRotation(const Angle& newRotation) { this->_target.SetRotation(newRotation); }
void Camera::SetZoom(const v2f& newZoom) { this->_target.SetScale(newZoom); }
void Camera::SetZoom(const f32& newZoomScalar) { this->_target.SetScale(newZoomScalar); }
void Camera::SetSkew(const v2f& newSkew) { this->_target.SetSkew(newSkew); }
void Camera::SetPosition(const v2x& newPosition)
{
	this->_fixedTargetPosition = newPosition;
	this->_target.SetTranslation(v2f(newPosition));
}

void Camera::Rotate(const Angle& rotationChange) { this->_target.Rotate(rotationChange); }
void Camera::Zoom(const v2f& zoomChange) { this->_target.Scale(zoomChange); }
void Camera::Zoom(const f32& zoomChangeScalar) { this->_target.Scale(zoomChangeScalar); }
void Camera::Skew(const v2f& skewChange) { this->_target.Skew(skewChange); }
void Camera::Move(const v2x& positionChange)
{
	this->_fixedTargetPosition += positionChange;
	this->_target.Translate(v2f(positionChange));
}

const Transform& Camera::GetTarget() const { return this->_target; }
void             Camera::SetTarget(const Transform& newTarget) { this->_target = newTarget; }

const Angle& Camera::GetTargetRotation() const { return this->_target.GetRotation(); }
const v2f& Camera::GetTargetZoom() const { return this->_target.GetScale(); }
const v2f& Camera::GetTargetSkew() const { return this->_target.GetSkew(); }
const v2x& Camera::GetTargetPosition() const { return this->_fixedTargetPosition; }

bool Camera::IsSpeedLimited() const { return this->_speedLimited; }
void Camera::SetSpeedLimited(const bool speedLimited) { this->_speedLimited = speedLimited; }

GameObject* Camera::GetFollowTarget() const { return this->_followTarget; }
void        Camera::SetFollowTarget(GameObject* followTarget) { this->_followTarget = followTarget; }


bool Camera::IsAtTarget() const { return this->_transform == this->_target; }

void Camera::Update()
{
	if (this->_transform == this->_target)
		return;

	if (!this->_speedLimited)
	{
		this->_fixedPosition = this->_fixedTargetPosition;
		this->_transform = this->_target;
		return;
	}

	this->UpdateRotation();
	this->UpdateZoom();
	this->UpdateSkew();
	this->UpdatePosition();
}

void Camera::UpdateRotation()
{
	const Angle&                  rotation       = this->_transform.GetRotation();
	const Angle&                  targetRotation = this->_target.GetRotation();
	const std::partial_ordering comparison     = rotation <=> targetRotation;

	if (comparison == std::partial_ordering::equivalent)
		return;

	if (comparison == std::partial_ordering::less)
		this->_transform.SetRotation(std::min(rotation + this->_rotationSpeed, targetRotation));
	if (comparison == std::partial_ordering::greater)
		this->_transform.SetRotation(std::max(targetRotation, rotation - this->_rotationSpeed));
}

void Camera::UpdateZoom()
{
	const v2f&                  zoom        = this->_transform.GetScale();
	const v2f&                  targetZoom  = this->_target.GetScale();
	const std::partial_ordering comparisonX = zoom.X <=> targetZoom.X;
	const std::partial_ordering comparisonY = zoom.Y <=> targetZoom.Y;

	if (comparisonX == std::partial_ordering::equivalent && comparisonY == std::partial_ordering::equivalent)
		return;

	v2f newZoom = v2f::Zero;
	if (comparisonX == std::partial_ordering::less)
		newZoom.X = std::min(zoom.X + this->_zoomSpeed.X, targetZoom.X);
	if (comparisonX == std::partial_ordering::greater)
		newZoom.X = std::max(targetZoom.X, zoom.X - this->_zoomSpeed.X);

	if (comparisonY == std::partial_ordering::less)
		newZoom.Y = std::min(zoom.Y + this->_zoomSpeed.Y, targetZoom.Y);
	if (comparisonY == std::partial_ordering::greater)
		newZoom.Y = std::max(targetZoom.Y, zoom.Y - this->_zoomSpeed.Y);

	this->_transform.SetScale(newZoom);
}

void Camera::UpdateSkew()
{
	const v2f&                  skew        = this->_transform.GetSkew();
	const v2f&                  targetSkew  = this->_target.GetSkew();
	const std::partial_ordering comparisonX = skew.X <=> targetSkew.X;
	const std::partial_ordering comparisonY = skew.Y <=> targetSkew.Y;

	if (comparisonX == std::partial_ordering::equivalent && comparisonY == std::partial_ordering::equivalent)
		return;

	v2f newSkew = v2f::Zero;
	if (comparisonX == std::partial_ordering::less)
		newSkew.X = std::min(skew.X + this->_skewSpeed.X, targetSkew.X);
	if (comparisonX == std::partial_ordering::greater)
		newSkew.X = std::max(targetSkew.X, skew.X - this->_skewSpeed.X);

	if (comparisonY == std::partial_ordering::less)
		newSkew.Y = std::min(skew.Y + this->_skewSpeed.Y, targetSkew.Y);
	if (comparisonY == std::partial_ordering::greater)
		newSkew.Y = std::max(targetSkew.Y, skew.Y - this->_skewSpeed.Y);

	this->_transform.SetSkew(newSkew);
}

void Camera::UpdatePosition()
{
	const v2x&                  position       = this->_fixedPosition;
	const v2x&                  targetPosition = this->_fixedTargetPosition;
	const std::partial_ordering comparisonX    = position.X <=> targetPosition.X;
	const std::partial_ordering comparisonY    = position.Y <=> targetPosition.Y;

	if (comparisonX == std::partial_ordering::equivalent && comparisonY == std::partial_ordering::equivalent)
		return;

	v2x newPosition = position;
	if (comparisonX == std::partial_ordering::less)
		newPosition.X = std::min(position.X + this->_moveSpeed.X, targetPosition.X);
	if (comparisonX == std::partial_ordering::greater)
		newPosition.X = std::max(targetPosition.X, position.X - this->_moveSpeed.X);

	if (comparisonY == std::partial_ordering::less)
		newPosition.Y = std::min(position.Y + this->_moveSpeed.Y, targetPosition.Y);
	if (comparisonY == std::partial_ordering::greater)
		newPosition.Y = std::max(targetPosition.Y, position.Y - this->_moveSpeed.Y);

	this->_fixedPosition = newPosition;
	this->_transform.SetTranslation(v2f(newPosition));
}
}
