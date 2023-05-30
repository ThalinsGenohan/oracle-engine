//
// Created by Thalins on 10/31/22.
//

#include "Game.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "GameObject.h"
#include "Input.h"

namespace oter
{
AssetManager<Shader>    Game::ShaderManager;
AssetManager<Texture2D> Game::TextureManager;
AssetManager<Tileset>   Game::TilesetManager;
AssetManager<Tilemap>   Game::TilemapManager;

Game::Game(const u32 width, const u32 height)
	: _gameResolution(width, height),
	  _hudSize(this->_gameResolution.X, 16),
	  _worldViewSize(this->_gameResolution.X, this->_gameResolution.Y - this->_hudSize.Y),
	  _halfWorldViewSize(this->_worldViewSize.X / 2, this->_worldViewSize.Y / 2),
	  _camera(this->_worldViewSize),
	  _frameBuffer(this->_gameResolution) {}

Game::~Game()
{
	delete this->_player;
}

void Game::Init()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Init framebuffers
	this->_frameBuffer.Init();

	this->_camera.SetSpeedLimited(false);
	this->_camera.SetPosition(v2x(this->_halfWorldViewSize));
	this->_camera.Update();
	this->_camera.SetSpeedLimited(true);

	// Load assets
	// Load Shaders
	ShaderManager.Load("framebuffer", "assets/shaders/framebuffer.glsl");
	ShaderManager.Load("sprite", "assets/shaders/sprite.glsl");
	ShaderManager.Load("tilemap", "assets/shaders/tilemap.glsl");
	// Load Textures
	TextureManager.Load("link", "assets/sprites/link.yaml");
	// Load Tilesets
	TilesetManager.Load("tilesetAtlas", "assets/tilesets/horon-village.yaml");
	// Load Tilemaps
	TilemapManager.Load("horon-village-spring", "assets/maps/horon-village-spring.map")
	              .SetTilesetID(0);
	TilemapManager.Load("horon-village-summer", "assets/maps/horon-village-summer.map")
	              .SetTilesetID(1);
	/*TilemapManager.Load("horon-village-autumn", "assets/maps/horon-village-autumn.map")
	              .SetTilesetID(2);
	TilemapManager.Load("horon-village-winter", "assets/maps/horon-village-winter.map")
	              .SetTilesetID(3);*/

	// Prep assets
	const Shader* frameBufferShader = &ShaderManager.Get("framebuffer");
	frameBufferShader->Use();
	frameBufferShader->SetInt("uScreenTexture", 0);

	const Shader* spriteShader = &ShaderManager.Get("sprite");
	spriteShader->Use();
	spriteShader->SetMatrix3("uView", this->_camera.GetMatrix());
	spriteShader->SetVector2u("uResolution", this->_worldViewSize);

	const Shader* tilemapShader = &ShaderManager.Get("tilemap");
	tilemapShader->Use();
	tilemapShader->SetMatrix3("uView", this->_camera.GetMatrix());
	tilemapShader->SetVector2u("uResolution", this->_worldViewSize);
	tilemapShader->SetInt("uTileAtlas", 0);
	tilemapShader->SetVector2u("uTileSize", 16u, 16u);

	// Init renderers
	this->_frameBufferRenderer.Init();
	this->_spriteRenderer.Init();
	this->_tilemapRenderer.Init();

	this->_currentTilemap = &TilemapManager.Get("horon-village-spring");

	this->_player = new GameObject(TextureManager.Get("link"));
	this->_player->SetPosition(v2x(8.0_x));
	this->_player->PlayAnimation("stand-down");
	this->_player->Update(0.f);
}

void Game::ProcessInput(f32 dt)
{
	if (WasKeyJustPressed(GLFW_KEY_SPACE))
		this->_updatesPaused = !this->_updatesPaused;
	if (WasKeyJustPressed(GLFW_KEY_F) && this->_updatesPaused)
		this->_updateQueued = true;

	if (WasKeyJustPressed(GLFW_KEY_W))
		this->_player->PlayAnimation("walk-up");
	if (WasKeyJustPressed(GLFW_KEY_A))
		this->_player->PlayAnimation("walk-left");
	if (WasKeyJustPressed(GLFW_KEY_S))
		this->_player->PlayAnimation("walk-down");
	if (WasKeyJustPressed(GLFW_KEY_D))
		this->_player->PlayAnimation("walk-right");

	if (WasKeyJustReleased(GLFW_KEY_W))
		this->_player->PlayAnimation("stand-up");
	if (WasKeyJustReleased(GLFW_KEY_A))
		this->_player->PlayAnimation("stand-left");
	if (WasKeyJustReleased(GLFW_KEY_S))
		this->_player->PlayAnimation("stand-down");
	if (WasKeyJustReleased(GLFW_KEY_D))
		this->_player->PlayAnimation("stand-right");

	if (!this->_cameraScreenTransition)
	{
		v2i playerDirection = v2i::Zero;
		if (GetKeyState(GLFW_KEY_W))
			playerDirection.Y += -1;
		if (GetKeyState(GLFW_KEY_A))
			playerDirection.X += -1;
		if (GetKeyState(GLFW_KEY_S))
			playerDirection.Y += 1;
		if (GetKeyState(GLFW_KEY_D))
			playerDirection.X += 1;

		if (playerDirection != v2i::Zero)
		{
			v2x playerMoveSpeed = playerDirection.X != 0 && playerDirection.Y != 0
				                      ? this->_playerDiagonalMoveSpeed
				                      : this->_playerNormalMoveSpeed;

			i32x8 movementX = playerMoveSpeed.X * playerDirection.X;
			i32x8 movementY = playerMoveSpeed.Y * playerDirection.Y;
			v2x movement = v2x(movementX, movementY);

			this->_player->Move(movement);
		}
	}

	if (WasKeyJustPressed(GLFW_KEY_UP))
		this->_player->Move(v2x(0.0_x, -1.0_x));
	if (WasKeyJustPressed(GLFW_KEY_DOWN))
		this->_player->Move(v2x(0.0_x, 1.0_x));
	if (WasKeyJustPressed(GLFW_KEY_LEFT))
		this->_player->Move(v2x(-1.0_x, 0.0_x));
	if (WasKeyJustPressed(GLFW_KEY_RIGHT))
		this->_player->Move(v2x(1.0_x, 0.0_x));

	if (WasKeyJustPressed(GLFW_KEY_1))
		this->SetCurrentTilemap("horon-village-spring");
	if (WasKeyJustPressed(GLFW_KEY_2))
		this->SetCurrentTilemap("horon-village-summer");
	if (WasKeyJustPressed(GLFW_KEY_3))
		this->SetCurrentTilemap("horon-village-autumn");
	if (WasKeyJustPressed(GLFW_KEY_4))
		this->SetCurrentTilemap("horon-village-winter");

	if (GetKeyState(GLFW_KEY_I))
		this->_camera.Move(v2x(0.0_x, -1.0_x));
	if (GetKeyState(GLFW_KEY_K))
		this->_camera.Move(v2x(0.0_x, 1.0_x));
	if (GetKeyState(GLFW_KEY_J))
		this->_camera.Move(v2x(-1.0_x, 0.0_x));
	if (GetKeyState(GLFW_KEY_L))
		this->_camera.Move(v2x(1.0_x, 0.0_x));
	if (GetKeyState(GLFW_KEY_U))
		this->_camera.Rotate(-1.0_d);
	if (GetKeyState(GLFW_KEY_O))
		this->_camera.Rotate(1.0_d);

	UpdateKeys();
}

void Game::Update(const f32 dt)
{
	if (this->_updatesPaused && !this->_updateQueued)
		return;
	if (this->_updateQueued)
		this->_updateQueued = false;

	if (this->_screenTransitionCounter == 0)
		this->_camera.Update();

	if (this->_screenTransitionCounter == 0)
		this->_player->Update(dt);

	if (this->_cameraScreenTransition)
		this->HandleCameraScreenTransition();
	else
	{
		const v2x& playerPos = this->_player->GetPosition();
		const v2x& cameraPos = this->_camera.GetPosition();
		const v2x playerScreenPos = v2x(playerPos - cameraPos + this->_halfWorldViewSize);

		v2i direction = v2i::Zero;
		if (playerScreenPos.Y <= this->_cameraTransitionThreshold.Y)
			direction.Y = -1;
		if (playerScreenPos.Y.Whole >= this->_worldViewSize.Y - this->_cameraTransitionThreshold.Y.Whole)
			direction.Y = 1;
		if (playerScreenPos.X <= this->_cameraTransitionThreshold.X)
			direction.X = -1;
		if (playerScreenPos.X.Whole >= this->_worldViewSize.X - this->_cameraTransitionThreshold.X.Whole)
			direction.X = 1;

		if (direction != v2i::Zero)
		{
			this->_cameraScreenTransition = true;

			this->_player->SetSpeedLimited(true);
			this->_player->SetMoveSpeed(this->_playerTransitionMoveSpeed);
			this->_player->Move(v2x(i32x8(15 * direction.X), i32x8(16 * direction.Y)));

			this->_camera.Move(v2x(i32x8(this->_worldViewSize.X * direction.X), i32x8(this->_worldViewSize.Y * direction.Y)));

			this->_screenTransitionCounter = 11;
		}
	}

	ShaderManager.Get("tilemap").SetMatrix3("uView", this->_camera.GetMatrix().GetInverse(), true);
	ShaderManager.Get("sprite").SetMatrix3("uView", this->_camera.GetMatrix().GetInverse(), true);

	this->_frameCounter++;
}

void Game::Render()
{
	this->_frameBuffer.Bind();

	// Render world
	glViewport(0, 0, this->_worldViewSize.X, this->_worldViewSize.Y);
	this->_frameBuffer.Clear(1.f, 1.f, 1.f);
	this->_tilemapRenderer.Draw(*this->_currentTilemap, ShaderManager.Get("tilemap"));
	this->_spriteRenderer.Draw(*this->_player, ShaderManager.Get("sprite"));

	// Render HUD
	glViewport(0, this->_worldViewSize.Y, this->_hudSize.X, this->_hudSize.Y);

	// Unbind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}

void Game::Draw()
{
	// Render framebuffers to screen
	_frameBufferRenderer.Draw(this->_frameBuffer, ShaderManager.Get("framebuffer"));
}

Tilemap* Game::GetCurrentTilemap() const
{
	return this->_currentTilemap;
}

void Game::SetCurrentTilemap(const std::string& name)
{
	this->_currentTilemap = &TilemapManager.Get(name);
	this->_currentTilemap->SetTileUpdate(true);
}

void Game::HandleCameraScreenTransition()
{
	if (this->_screenTransitionCounter > 0)
	{
		this->_screenTransitionCounter--;
		return;
	}

	if (this->_camera.IsAtTarget())
	{
		this->_cameraScreenTransition = false;
		this->_player->SetSpeedLimited(false);
	}
}
}
