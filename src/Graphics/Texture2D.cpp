//
// Created by Thalins on 10/30/22.
//
#include "Graphics/Texture2D.h"

#include <glad/glad.h>

namespace oracle
{
Texture2D::Texture2D()
{
	glGenTextures(1, &this->_id);
}

void Texture2D::Generate(const u32 width, const u32 height, const u8* data)
{
	this->Generate(v2u(width, height), data);
}

void Texture2D::Generate(const v2u& size, const u8* data)
{
	this->_size = size;

	glBindTexture(GL_TEXTURE_2D, this->_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->_size.X, this->_size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture2D::Delete()
{
	glDeleteTextures(1, &this->_id);
	this->_id = 0;
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->_id);
}

u32 Texture2D::GetID() const
{
	return this->_id;
}

const v2u& Texture2D::GetTextureSize() const
{
	return this->_size;
}

const v2u& Texture2D::GetFrameSize() const
{
	return this->_frameSize;
}

void Texture2D::SetFrameSize(const v2u& frameSize)
{
	this->_frameSize = frameSize;
}

const std::unordered_map<std::string, Texture2D::Animation>& Texture2D::GetAnimations() const
{
	return this->_animations;
}

void Texture2D::SetAnimations(const std::unordered_map<std::string, Animation>& animations)
{
	this->_animations = animations;
}

const Texture2D::Animation& Texture2D::GetAnimation(const std::string& animationName) const
{
	return this->_animations.at(animationName);
}

void Texture2D::SetAnimation(const std::string& animationName, const Animation& animation)
{
	this->_animations[animationName] = animation;
}

const Texture2D::AnimationFrame& Texture2D::GetAnimationFrame(const std::string& animationName, const u32 frameNumber) const
{
	return this->_animations.at(animationName).at(frameNumber);
}

void Texture2D::SetAnimationFrame(const std::string& animationName, const u32 frameNumber, const AnimationFrame& frame)
{
	this->_animations[animationName][frameNumber] = frame;
}
}
