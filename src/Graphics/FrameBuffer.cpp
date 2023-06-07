//
// Created by Thalins on 1/29/23.
//

#include "Graphics/FrameBuffer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace oracle
{
FrameBuffer::FrameBuffer(const v2u& size) : _size(size) {}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &this->_framebufferID);
	glDeleteTextures(1, &this->_textureID);
	//glDeleteRenderbuffers(1, &this->_renderbufferID);
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &this->_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->_framebufferID);

	glGenTextures(1, &this->_textureID);
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_size.X, this->_size.Y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glBindTexture(GL_TEXTURE_2D, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_textureID, 0);

	//glGenRenderbuffers(1, &this->_renderbufferID);
	//glBindRenderbuffer(GL_RENDERBUFFER, this->_renderbufferID);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->_size.X, this->_size.Y);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->_renderbufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		spdlog::error("Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->_framebufferID);
}

void FrameBuffer::BindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
}

void FrameBuffer::Clear(const f32 r, const f32 g, const f32 b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);
}
}
