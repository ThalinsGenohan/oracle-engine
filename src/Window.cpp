//
// Created by Thalins on 10/29/22.
//
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Input.h"

namespace oter
{
Game* Window::_game;

Window& Window::Create(const u16 width, const u16 height, const std::string& title)
{
	auto* window = new Window(width, height, title);
	return *window;
}

Window::Window(const u16 width, const u16 height, const std::string& title)
	: _glfwWindow(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr))
{
	if (this->_glfwWindow == nullptr)
	{
		spdlog::critical("Failed to create GLFW window");
		return;
	}
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(this->_glfwWindow, ChangeFramebufferSize);
	glfwSetKeyCallback(this->_glfwWindow, HandleKey);
}

Window::~Window()
{
	glfwDestroyWindow(this->_glfwWindow);
}

void Window::MakeCurrent() const
{
	glfwMakeContextCurrent(this->_glfwWindow);
}

void Window::ChangeFramebufferSize(GLFWwindow* window, const i32 width, const i32 height)
{
	glViewport(0, 0, width, height);
}

void Window::HandleKey(GLFWwindow* window, const i32 key, i32 scanCode, const i32 action, i32 mode)
{
	switch (action)
	{
	case GLFW_PRESS:
		SetKeyJustPressed(static_cast<u32>(key));
		break;
	case GLFW_RELEASE:
		SetKeyJustReleased(static_cast<u32>(key));
		break;
	default:
		break;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			SetKeyJustPressed(key);
		if (action == GLFW_RELEASE)
			SetKeyJustReleased(key);
	}
}

void Window::SetGame(Game* game)
{
	_game = game;
}

bool Window::IsClosing() const
{
	return glfwWindowShouldClose(this->_glfwWindow);
}

void Window::Clear(const f32 r, const f32 g, const f32 b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Draw(void* object) {}

void Window::Display() const
{
	glfwSwapBuffers(this->_glfwWindow);
}
}
