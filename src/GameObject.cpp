//
// Created by Thalins on 12/31/22.
//

#include "GameObject.h"

namespace oter
{
GameObject::GameObject(Texture2D& texture) : _texture(texture) {}

const Transform& GameObject::GetTransform() const { return this->_transform; }
void             GameObject::SetTransform(const Transform& newTransform) { this->_transform = newTransform; }
const m3f&       GameObject::GetMatrix() { return this->_transform.GetMatrix(); }

const Angle& GameObject::GetRotation() const { return this->_transform.GetRotation(); }
const v2f& GameObject::GetScale() const { return this->_transform.GetScale(); }
const v2f& GameObject::GetSkew() const { return this->_transform.GetSkew(); }
const v2x& GameObject::GetPosition() const { return this->_fixedPosition; }

void GameObject::SetRotation(const Angle& newRotation) { this->_target.SetRotation(newRotation); }
void GameObject::SetScale(const v2f& newScale) { this->_target.SetScale(newScale); }
void GameObject::SetScale(const f32& newScaleScalar) { this->_target.SetScale(newScaleScalar); }
void GameObject::SetSkew(const v2f& newSkew) { this->_target.SetSkew(newSkew); }
void GameObject::SetPosition(const v2x& newPosition)
{
	this->_fixedTargetPosition = newPosition;
	this->_target.SetTranslation(v2f(newPosition));
}

void GameObject::Rotate(const Angle& rotationChange) { this->_target.Rotate(rotationChange); }
void GameObject::Scale(const v2f& scaleChange) { this->_target.Scale(scaleChange); }
void GameObject::Scale(const f32& scaleChangeScalar) { this->_target.Scale(scaleChangeScalar); }
void GameObject::Skew(const v2f& skewChange) { this->_target.Skew(skewChange); }
void GameObject::Move(const v2x& positionChange)
{
	this->_fixedTargetPosition += positionChange;
	this->_target.Translate(v2f(positionChange));
}

const Transform& GameObject::GetTarget() const { return this->_target; }
void             GameObject::SetTarget(const Transform& newTarget) { this->_target = newTarget; }

const Angle& GameObject::GetTargetRotation() const { return this->_target.GetRotation(); }
const v2f& GameObject::GetTargetZoom() const { return this->_target.GetScale(); }
const v2f& GameObject::GetTargetSkew() const { return this->_target.GetSkew(); }
const v2x& GameObject::GetTargetPosition() const { return this->_fixedTargetPosition; }

bool GameObject::IsAtTarget() const { return this->_transform == this->_target; }

bool GameObject::IsSpeedLimited() const { return this->_speedLimited; }
void GameObject::SetSpeedLimited(const bool speedLimited) { this->_speedLimited = speedLimited; }

void GameObject::SetRotationSpeed(const f32& rotationSpeed) { this->_rotationSpeed = rotationSpeed; }
void GameObject::SetScaleSpeed(const v2f& scaleSpeed) { this->_scaleSpeed = scaleSpeed; }
void GameObject::SetSkewSpeed(const v2f& skewSpeed) { this->_skewSpeed = skewSpeed; }
void GameObject::SetMoveSpeed(const v2x& moveSpeed) { this->_moveSpeed = moveSpeed; }

bool GameObject::IsAnimationUpdatePending() const { return this->_animationUpdatePending; }

void GameObject::SetAnimationUpdatePending(const bool update) { this->_animationUpdatePending = update; }

const v2u& GameObject::GetFramePosition() const { return this->_framePosition; }

const std::string& GameObject::GetCurrentAnimationID() const
{
	return this->_currentAnimationID;
}

u32 GameObject::GetCurrentAnimationFrameNumber() const
{
	return this->_currentFrameNumber;
}

const std::vector<Texture2D::AnimationFrame>& GameObject::GetCurrentAnimation() const
{
	return this->_texture.GetAnimation(this->_currentAnimationID);
}

const Texture2D::AnimationFrame& GameObject::GetCurrentAnimationFrame()
{
	const Texture2D::Animation& animation = this->_texture.GetAnimation(this->_currentAnimationID);
	if (this->_currentFrameNumber >= animation.size())
		this->_currentFrameNumber = 0;

	return animation[this->_currentFrameNumber];
}

void GameObject::PlayAnimation(const std::string& animation)
{
	this->_currentAnimationID = animation;
	this->_currentFrameNumber = 0;
	this->_frameTimerMs = 0.f;
	this->SetAnimationUpdatePending(true);
}

const Texture2D& GameObject::GetTexture() const
{
	return this->_texture;
}

void GameObject::Update(const float deltaMs)
{
	this->UpdateRotation();
	this->UpdateScale();
	this->UpdateSkew();
	this->UpdatePosition();

	const Texture2D::Animation& animation = this->GetCurrentAnimation();

	if (animation.size() == 1)
		return;

	const auto& [framePosition, milliseconds] = animation[this->_currentFrameNumber];

	if (this->_frameTimerMs >= milliseconds)
	{
		this->_frameTimerMs -= milliseconds;
		this->_currentFrameNumber++;
		if (this->_currentFrameNumber == animation.size())
		{
			this->_currentFrameNumber = 0;
		}
		this->_framePosition = framePosition;
		this->SetAnimationUpdatePending(true);
	}

	this->_frameTimerMs += deltaMs;
}

void GameObject::UpdateRotation()
{
	if (!this->_speedLimited)
	{
		this->_transform.SetRotation(this->_target.GetRotation());
		return;
	}

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

void GameObject::UpdateScale()
{
	if (!this->_speedLimited)
	{
		this->_transform.SetScale(this->_target.GetScale());
		return;
	}

	const v2f&                  scale       = this->_transform.GetScale();
	const v2f&                  targetScale = this->_target.GetScale();
	const std::partial_ordering comparisonX = scale.X <=> targetScale.X;
	const std::partial_ordering comparisonY = scale.Y <=> targetScale.Y;

	if (comparisonX == std::partial_ordering::equivalent && comparisonY == std::partial_ordering::equivalent)
		return;

	v2f newScale = v2f::Zero;
	if (comparisonX == std::partial_ordering::less)
		newScale.X = std::min(scale.X + this->_scaleSpeed.X, targetScale.X);
	if (comparisonX == std::partial_ordering::greater)
		newScale.X = std::max(targetScale.X, scale.X - this->_scaleSpeed.X);

	if (comparisonY == std::partial_ordering::less)
		newScale.Y = std::min(scale.Y + this->_scaleSpeed.Y, targetScale.Y);
	if (comparisonY == std::partial_ordering::greater)
		newScale.Y = std::max(targetScale.Y, scale.Y - this->_scaleSpeed.Y);

	this->_transform.SetScale(newScale);
}

void GameObject::UpdateSkew()
{
	if (!this->_speedLimited)
	{
		this->_transform.SetSkew(this->_target.GetSkew());
		return;
	}

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

void GameObject::UpdatePosition()
{
	if (!this->_speedLimited)
	{
		this->_fixedPosition = this->_fixedTargetPosition;
		this->_transform.SetTranslation(this->_target.GetTranslation());
		return;
	}

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
