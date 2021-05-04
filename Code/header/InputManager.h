#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

// when a gameobject needs input information, 
// register a shared_ptr from Game::access() into the corresponding component
// and the register call should be put in start()

struct KeyState {
	int glfw_order;
	bool last;
	bool current;
};

enum GameType {
	FPS_GAME
};

class InputManager {
public:
	InputManager(GLFWwindow* window, GameType type = FPS_GAME);

	// keyboard part
	void process_input(float delta);

	void register_key(std::string key, int glfw_order);
	void logout_key(std::string key);

	bool exit_window();

	bool is_press(std::string key);
	bool is_hold( std::string key );
	bool is_release(std::string key);
	bool is_unhold(std::string key);

	// cursor part
	float* get_cursor_move();

private:
	GLFWwindow* window;
	std::unordered_map< std::string, KeyState > key_list;

	bool first_cursor_move;
	float last_cursor_pos[2];
	float cursor_move[2];

	void initialize(GameType type);

	static InputManager* access(InputManager* manager = nullptr);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y);
};
