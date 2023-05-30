//
// Created by Thalins on 1/29/23.
//

#include "Graphics/Renderer.h"

#include <OtterML/Graphics/Shader.hpp>
#include <OtterML/System/Vector2.hpp>

#include "Game.h"
#include "GameObject.h"
#include "Tilemap.h"
#include "Graphics/FrameBuffer.h"

namespace oter
{
template <>
void Renderer<FrameBuffer>::Draw(FrameBuffer& frameBuffer, const Shader& shader)
{
	constexpr float vertices[16] = {
		-1.f, -1.f, 0.f, 0.f,
		-1.f, 1.f, 0.f, 1.f,
		1.f, -1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f,
	};

	glBindVertexArray(this->_vao);

	// Create VBO if needed
	if (!this->_vbos.contains(&frameBuffer))
	{
		this->_vbos[&frameBuffer] = NULL;
		glGenBuffers(1, &this->_vbos[&frameBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&frameBuffer]);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof vertices[0], reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&frameBuffer]);

	// Set shader uniforms
	shader.Use();

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	frameBuffer.BindTexture();

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

template <>
void Renderer<GameObject>::Draw(GameObject& gameObject, const Shader& shader)
{
	const Texture2D& texture                       = gameObject.GetTexture();
	const v2u&       frameSize                     = texture.GetFrameSize();
	const auto&      [framePosition, milliseconds] = gameObject.GetCurrentAnimationFrame();

	const glm::uvec2 vertices[4] = {
		static_cast<glm::uvec2>(framePosition + v2u(0, frameSize.Y)),
		static_cast<glm::uvec2>(framePosition),
		static_cast<glm::uvec2>(framePosition + frameSize),
		static_cast<glm::uvec2>(framePosition + v2u(frameSize.X, 0)),
	};

	glBindVertexArray(this->_vao);

	// Create VBO if needed
	if (!this->_vbos.contains(&gameObject))
	{
		this->_vbos[&gameObject] = NULL;
		glGenBuffers(1, &this->_vbos[&gameObject]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&gameObject]);
		glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);
		glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof vertices[0], reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		gameObject.SetAnimationUpdatePending(true);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&gameObject]);

	if (gameObject.IsAnimationUpdatePending())
	{
		// Update animation state
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof vertices, vertices);
		glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof vertices[0], reinterpret_cast<void*>(0));
		gameObject.SetAnimationUpdatePending(false);
	}

	// Set shader uniforms
	shader.Use();
	shader.SetVector2u("uSpriteSize", frameSize);
	shader.SetMatrix3("uSpriteMatrix", gameObject.GetMatrix());

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Unbind all
	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

template <>
void Renderer<Tilemap>::Draw(Tilemap& tilemap, const Shader& shader)
{
	const std::vector<u16>& tiles = tilemap.GetTiles();
	glBindVertexArray(this->_vao);

	// Create VBO if needed
	if (!this->_vbos.contains(&tilemap))
	{
		this->_vbos[&tilemap] = NULL;
		glGenBuffers(1, &this->_vbos[&tilemap]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&tilemap]);
		glBufferData(GL_ARRAY_BUFFER, tilemap.GetTileCount() * sizeof tiles[0], tiles.data(), GL_DYNAMIC_DRAW);
		glVertexAttribIPointer(0, 1, GL_UNSIGNED_SHORT, sizeof tiles[0], reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		tilemap.SetTileUpdate(true);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbos[&tilemap]);

	if (tilemap.IsTileUpdatePending())
	{
		// Update tile indexes
		glBufferSubData(GL_ARRAY_BUFFER, 0, tilemap.GetTileCount() * sizeof tiles[0], tiles.data());
		glVertexAttribIPointer(0, 1, GL_UNSIGNED_SHORT, sizeof tiles[0], reinterpret_cast<void*>(0));
		tilemap.SetTileUpdate(false);
	}

	// Set shader uniforms
	shader.Use();
	shader.SetUInt("uMapWidth", tilemap.GetMapSize().X);
	shader.SetVector2i("uPosition", tilemap.GetPosition());
	shader.SetUInt("uTilesetID", tilemap.GetTilesetID());

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	Game::TilesetManager.Get("tilesetAtlas").Bind();

	// Draw
	glDrawArrays(GL_POINTS, 0, tilemap.GetTileCount());

	// Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
}
