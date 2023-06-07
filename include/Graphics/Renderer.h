//
// Created by Thalins on 1/29/23.
//

#ifndef ORACLES_RENDERER_H
#define ORACLES_RENDERER_H

#include <unordered_map>
#include <glad/glad.h>

#include "types.h"

namespace oter
{
	class Shader;
}

namespace oracle
{
template <typename T>
class Renderer
{
public:
	Renderer() {}

	~Renderer()
	{
		std::vector<u32> buffers = std::vector<u32>(this->_vbos.size());
		for (auto iter = this->_vbos.begin(); iter != this->_vbos.end(); ++iter)
		{
			buffers.push_back(iter->second);
		}

		glDeleteBuffers(this->_vbos.size(), buffers.data());
		glDeleteVertexArrays(1, &this->_vao);
	}

	void Init()
	{
		glGenVertexArrays(1, &this->_vao);
		glBindVertexArray(this->_vao);
	}

	void Draw(T&, const oter::Shader& shader);

private:
	u32                         _vao  = 0;
	std::unordered_map<T*, u32> _vbos = {};
};
}

#endif
