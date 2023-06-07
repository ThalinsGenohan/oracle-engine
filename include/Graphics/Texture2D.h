//
// Created by Thalins on 10/30/22.
//

#ifndef ORACLES_TEXTURE2D_H
#define ORACLES_TEXTURE2D_H

#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace oracle
{
class Texture2D
{
public:
	struct AnimationFrame;

	using Animation = std::vector<AnimationFrame>;

	Texture2D();

	void Generate(u32 width, u32 height, const u8* data);
	void Generate(const v2u& size, const u8* data);

	void Delete();

	void Bind() const;

	u32 GetID() const;

	const v2u& GetTextureSize() const;

	const v2u& GetFrameSize() const;
	void       SetFrameSize(const v2u& frameSize);

	const std::unordered_map<std::string, Animation>& GetAnimations() const;
	void SetAnimations(const std::unordered_map<std::string, Animation>& animations);

	const Animation& GetAnimation(const std::string& animationName) const;
	void             SetAnimation(const std::string& animationName, const Animation& animation);

	const AnimationFrame& GetAnimationFrame(const std::string& animationName, u32 frameNumber) const;
	void SetAnimationFrame(const std::string& animationName, u32 frameNumber, const AnimationFrame& frame);

private:
	u32 _id        = 0;
	v2u _size      = v2u::Zero;
	v2u _frameSize = v2u::Zero;

	std::unordered_map<std::string, Animation> _animations;
};

struct Texture2D::AnimationFrame
{
public:
	v2u framePosition = v2u::Zero;
	f32 milliseconds  = 0.f;
};
}

#endif //ORACLES_TEXTURE2D_H
