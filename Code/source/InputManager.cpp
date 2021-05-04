#include "InputManager.h"
#include <iostream>

InputManager::InputManager(GLFWwindow* window, GameType type)
{
	this->window = window;
	initialize(type);

	first_cursor_move = true;

	access(this);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback );
	glfwSetCursorPosCallback(window, cursor_move_callback);
}

void InputManager::initialize(GameType type)
{
	switch (type) {
	case FPS_GAME:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		register_key("W", GLFW_KEY_W);
		register_key("A", GLFW_KEY_A);
		register_key("S", GLFW_KEY_S);
		register_key("D", GLFW_KEY_D);
		register_key("Space", GLFW_KEY_SPACE);
		break;
	}
}

bool InputManager::exit_window()
{
	return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

void InputManager::process_input(float delta)
{
	// exit game
	for (auto it = key_list.begin(); it != key_list.end(); ++it)
	{
		it->second.last = it->second.current;
		it->second.current = glfwGetKey(window, it->second.glfw_order) == GLFW_PRESS;
	}
	// something else maybe
}

// keyboard part
void InputManager::register_key(std::string key, int glfw_order)
{
	if (key_list.find(key) == key_list.end())
	{
		KeyState key_state;
		key_state.glfw_order = glfw_order;
		key_state.last = false;
		key_state.current = false;

		key_list.insert({ key, key_state });
	}
	else
	{
		std::cout << "Rigester key failed: key " << key << " has been registered. " << std::endl;
	}
}

void InputManager::logout_key(std::string key)
{
	if (key_list.find(key) != key_list.end())
	{
		key_list.erase(key);
	}
	else
	{
		std::cout << "Logout key failed: key " << key << " does not exist. " << std::endl;
	}
}

bool InputManager::is_press(std::string key)
{
	auto it = key_list.find(key);

	if (it != key_list.end())
	{
		return (!it->second.last) && (it->second.current);
	}
	return false;
}

bool InputManager::is_hold(std::string key)
{
	auto it = key_list.find(key);

	if (it != key_list.end())
	{
		return (it->second.current);
	}
	return false;
}

bool InputManager::is_release(std::string key)
{
	auto it = key_list.find(key);

	if (it != key_list.end())
	{
		return ( it->second.last ) && ( ! it->second.current );
	}
	return false;
}

bool InputManager::is_unhold(std::string key)
{
	auto it = key_list.find(key);

	if (it != key_list.end())
	{
		return (! it->second.current);
	}
	return false;
}

// cursor part
float* InputManager::get_cursor_move()
{
	return cursor_move;
}


// static method
InputManager* InputManager::access(InputManager* manager)
{
	static InputManager* instance = nullptr;

	if (instance == nullptr)
		instance = manager;

	return instance;
}

void InputManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void InputManager::cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y)
{
	InputManager* manager = access();
	if (manager->first_cursor_move)
	{
		manager->last_cursor_pos[0] = (float) cursor_x;
		manager->last_cursor_pos[1] = (float) cursor_y;

		manager->first_cursor_move = false;
	}

	manager->cursor_move[0] = (float) cursor_x - manager->last_cursor_pos[0];
	manager->cursor_move[1] = (float) cursor_y - manager->last_cursor_pos[1];

	manager->last_cursor_pos[0] = (float)cursor_x;
	manager->last_cursor_pos[1] = (float)cursor_y;
}