#include "InputManager.h"
#include <iostream>

InputManager::InputManager(GLFWwindow* window, GameType type)
{
	this->window = window;
	initialize(type);
}

void InputManager::initialize(GameType type)
{
	switch (type) {
	case FPS_GAME:
		register_key("W", GLFW_KEY_W);
		register_key("A", GLFW_KEY_A);
		register_key("S", GLFW_KEY_S);
		register_key("D", GLFW_KEY_D);
		register_key("Space", GLFW_KEY_SPACE);
		break;
	}
}

void InputManager::process_input(float delta)
{
	for (auto it = key_list.begin(); it != key_list.end(); ++it)
	{
		it->second.last = it->second.current;
		it->second.current = glfwGetKey(window, it->second.glfw_order) == GLFW_PRESS;
	}
	// something else maybe
}

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

