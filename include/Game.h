//
// Created by Thalins on 10/29/22.
//

#ifndef ORACLES_GAME_H
#define ORACLES_GAME_H

#include <string>
#include <OtterML/Graphics/Shader.hpp>

#include "AssetManager.h"
#include "Tilemap.h"
#include "Tileset.h"
#include "types.h"
#include "Graphics/Camera.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture2D.h"

namespace oracle
{
class GameObject;

class Game
{
public:
	static AssetManager<oter::Shader> ShaderManager;
	static AssetManager<Texture2D>    TextureManager;
	static AssetManager<Tileset>      TilesetManager;
	static AssetManager<Tilemap>      TilemapManager;

	Game(u32 width, u32 height);
	~Game();

	void Init();
	void ProcessInput(f32 dt);
	void Update(f32 dt);
	void Render();
	void Draw();

	Tilemap* GetCurrentTilemap() const;
	void     SetCurrentTilemap(const std::string& name);

private:
	const v2x _cameraTransitionThreshold = v2x(i32x8(6), i32x8(7));

	v2u _gameResolution = v2u::Zero;

	v2u _hudSize           = v2u::Zero;
	v2u _worldViewSize     = v2u::Zero;
	v2u _halfWorldViewSize = v2u::Zero;

	Camera _camera;
	bool   _cameraScreenTransition  = false;
	u8     _screenTransitionCounter = 0;

	Renderer<FrameBuffer> _frameBufferRenderer = Renderer<FrameBuffer>();
	Renderer<GameObject>  _spriteRenderer      = Renderer<GameObject>();
	Renderer<Tilemap>     _tilemapRenderer     = Renderer<Tilemap>();

	Tilemap* _currentTilemap = nullptr;

	const v2x   _playerTransitionMoveSpeed = v2x(i32x8(0, 0x60), i32x8(0, 0x80));
	const v2x   _playerNormalMoveSpeed     = v2x(i32x8(1, 0x00), i32x8(1, 0x00));
	const v2x   _playerDiagonalMoveSpeed   = v2x(i32x8(0, 0xB4), i32x8(0, 0xB4));
	GameObject* _player                    = nullptr;

	u64 _frameCounter = 0;

	FrameBuffer _frameBuffer;

	bool _updatesPaused = false;
	bool _updateQueued  = false;

	void HandleCameraScreenTransition();
};
}

#endif //ORACLES_GAME_H
