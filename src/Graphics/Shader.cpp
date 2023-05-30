//
// Created by Thalins on 10/29/22.
//
#include "Graphics/Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace oter
{
Shader::Shader() {}

Shader::~Shader()
{
	_id = 0;
}

void Shader::Delete()
{
	glDeleteProgram(this->_id);
	this->_id = 0;
}

void Shader::Use() const
{
	glUseProgram(this->_id);
}

void Shader::Compile(const std::string& vertSource, const std::string& fragSource)
{
	this->Compile(vertSource, "", fragSource);
}

void Shader::Compile(const std::string& vertSource, const std::string& geomSource, const std::string& fragSource)
{
	this->Compile(vertSource.c_str(), geomSource.c_str(), fragSource.c_str());
}

void Shader::Compile(const char* vertSource, const char* geomSource, const char* fragSource)
{
	// vertex
	const u32 vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, nullptr);
	glCompileShader(vert);
	CheckCompileErrors(vert, "VERTEX");

	// geometry
	u32 geom = 0;
	if (strcmp(geomSource, "") != 0)
	{
		geom = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom, 1, &geomSource, nullptr);
		glCompileShader(geom);
		CheckCompileErrors(geom, "GEOMETRY");
	}

	// fragment
	const u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, nullptr);
	glCompileShader(frag);
	CheckCompileErrors(frag, "FRAGMENT");

	// program
	this->_id = glCreateProgram();
	glAttachShader(this->_id, vert);
	if (geom != 0)
	{
		glAttachShader(this->_id, geom);
	}
	glAttachShader(this->_id, frag);
	glLinkProgram(this->_id);
	CheckCompileErrors(this->_id, "PROGRAM");

	glDeleteShader(vert);
	if (geom != 0)
		glDeleteShader(geom);
	glDeleteShader(frag);
}

void Shader::SetFloat(const char* name, const f32 value, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform1f(glGetUniformLocation(this->_id, name), value);
}

void Shader::SetVector2f(const char* name, const f32 x, const f32 y, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform2f(glGetUniformLocation(this->_id, name), x, y);
}

void Shader::SetVector2f(const char* name, const glm::vec2& value, const bool useProgram) const
{
	this->SetVector2f(name, value.x, value.y, useProgram);
}

void Shader::SetVector2f(const char* name, const v2f& value, const bool useProgram) const
{
	this->SetVector2f(name, value.X, value.Y, useProgram);
}

void Shader::SetVector3f(const char* name, const f32 x, const f32 y, const f32 z, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform3f(glGetUniformLocation(this->_id, name), x, y, z);
}

void Shader::SetVector3f(const char* name, const glm::vec3& value, const bool useProgram) const
{
	this->SetVector3f(name, value.x, value.y, value.z, useProgram);
}

void Shader::SetVector3f(const char* name, const v3f& value, const bool useProgram) const
{
	this->SetVector3f(name, value.X, value.Y, value.Z, useProgram);
}

void Shader::SetVector4f(const char* name, const f32 x, const f32 y, const f32 z, const f32 w,
                         const bool  useProgram
) const
{
	if (useProgram)
		this->Use();
	glUniform4f(glGetUniformLocation(this->_id, name), x, y, z, w);
}

void Shader::SetVector4f(const char* name, const glm::vec4& value, const bool useProgram) const
{
	this->SetVector4f(name, value.x, value.y, value.z, value.w, useProgram);
}

void Shader::SetInt(const char* name, const i32 value, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform1i(glGetUniformLocation(this->_id, name), value);
}

void Shader::SetVector2i(const char* name, const i32 x, const i32 y, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform2i(glGetUniformLocation(this->_id, name), x, y);
}

void Shader::SetVector2i(const char* name, const glm::ivec2& value, const bool useProgram) const
{
	this->SetVector2i(name, value.x, value.y, useProgram);
}

void Shader::SetVector2i(const char* name, const v2i& value, const bool useProgram) const
{
	this->SetVector2i(name, value.X, value.Y, useProgram);
}

void Shader::SetVector3i(const char* name, const i32 x, const i32 y, const i32 z, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform3i(glGetUniformLocation(this->_id, name), x, y, z);
}

void Shader::SetVector3i(const char* name, const glm::ivec3& value, const bool useProgram) const
{
	this->SetVector3i(name, value.x, value.y, value.z, useProgram);
}

void Shader::SetVector3i(const char* name, const v3i& value, const bool useProgram) const
{
	this->SetVector3i(name, value.X, value.Y, value.Z, useProgram);
}

void Shader::SetVector4i(const char* name, const i32 x, const i32 y, const i32 z, const i32 w, const bool useProgram
) const
{
	if (useProgram)
		this->Use();
	glUniform4i(glGetUniformLocation(this->_id, name), x, y, z, w);
}

void Shader::SetVector4i(const char* name, const glm::ivec4& value, const bool useProgram) const
{
	this->SetVector4i(name, value.x, value.y, value.z, value.w, useProgram);
}

void Shader::SetUInt(const char* name, const u32 value, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform1ui(glGetUniformLocation(this->_id, name), value);
}

void Shader::SetVector2u(const char* name, const u32 x, const u32 y, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform2ui(glGetUniformLocation(this->_id, name), x, y);
}

void Shader::SetVector2u(const char* name, const glm::uvec2& value, const bool useProgram) const
{
	this->SetVector2u(name, value.x, value.y, useProgram);
}

void Shader::SetVector2u(const char* name, const v2u& value, const bool useProgram) const
{
	this->SetVector2u(name, value.X, value.Y, useProgram);
}

void Shader::SetVector3u(const char* name, const u32 x, const u32 y, const u32 z, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniform3ui(glGetUniformLocation(this->_id, name), x, y, z);
}

void Shader::SetVector3u(const char* name, const glm::uvec3& value, const bool useProgram) const
{
	this->SetVector3u(name, value.x, value.y, value.z, useProgram);
}

void Shader::SetVector3u(const char* name, const v3u& value, const bool useProgram) const
{
	this->SetVector3u(name, value.X, value.Y, value.Z, useProgram);
}

void Shader::SetVector4u(const char* name, const u32 x, const u32 y, const u32 z, const u32 w, const bool useProgram
) const
{
	if (useProgram)
		this->Use();
	glUniform4ui(glGetUniformLocation(this->_id, name), x, y, z, w);
}

void Shader::SetVector4u(const char* name, const glm::uvec4& value, const bool useProgram) const
{
	this->SetVector4u(name, value.x, value.y, value.z, value.w, useProgram);
}

void Shader::SetMatrix3(const char* name, glm::mat3& matrix, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniformMatrix3fv(glGetUniformLocation(this->_id, name), 1, true, value_ptr(matrix));
}

void Shader::SetMatrix3(const char* name, const m3f& matrix, const bool useProgram) const
{
	glm::mat3 m = static_cast<glm::mat3>(matrix);
	this->SetMatrix3(name, m, useProgram);
}

void Shader::SetMatrix4(const char* name, glm::mat4& matrix, const bool useProgram) const
{
	if (useProgram)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->_id, name), 1, false, value_ptr(matrix));
}

Shader::operator u32() const
{
	return this->_id;
}

void Shader::CheckCompileErrors(const u32 object, const char* type)
{
	int  success;
	char infoLog[1024];
	if (strcmp(type, "PROGRAM") != 0)
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, nullptr, infoLog);
			spdlog::error("Shader compile-time error: Type: {}\n{}", type, infoLog);
		}
	} else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, nullptr, infoLog);
			spdlog::error("Shader link-time error: Type: {}\n{}", type, infoLog);
		}
	}
}

u32 Shader::GetID() const
{
	return this->_id;
}
}
