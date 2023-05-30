//
// Created by Thalins on 10/29/22.
//

#ifndef ORACLES_WINDOW_H
#define ORACLES_WINDOW_H

#include <string>

#include "Game.h"
#include "types.h"

class GLFWwindow;

namespace oter
{
class Window
{
public:
	static Window& Create(u16 width, u16 height, const std::string& title);
	Window(u16 width, u16 height, const std::string& title);
	~Window();

	static void SetGame(Game* game);

	void MakeCurrent() const;

	bool IsClosing() const;

	static void Clear(f32 r, f32 g, f32 b);
	static void Draw(void* object);
	void        Display() const;

private:
	static Game* _game;

	GLFWwindow* _glfwWindow = nullptr;

	static void ChangeFramebufferSize(GLFWwindow* window, i32 width, i32 height);
	static void HandleKey(GLFWwindow* window, i32 key, i32 scanCode, i32 action, i32 mode);
};
}

#endif //ORACLES_WINDOW_H
