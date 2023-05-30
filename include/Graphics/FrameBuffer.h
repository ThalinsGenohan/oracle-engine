//
// Created by Thalins on 1/29/23.
//

#ifndef ORACLES_FRAMEBUFFER_H
#define ORACLES_FRAMEBUFFER_H

#include "types.h"

namespace oter
{
class FrameBuffer
{
public:
	explicit FrameBuffer(const v2u& size);
	~FrameBuffer();

	void Init();

	void Bind() const;

	void BindTexture() const;

	static void Clear(f32 r, f32 g, f32 b);

private:
	v2u _size = v2u::Zero;

	u32 _framebufferID = NULL;
	u32 _textureID     = NULL;
	//u32 _renderbufferID;
};
}

#endif // ORACLES_FRAMEBUFFER_H
