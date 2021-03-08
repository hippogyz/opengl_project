#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class InputController {
public:
	InputController();
	
	void initialize(GLFWwindow* window);

	void update(float delta_time);

	void processInput(float delta_time);
	void cursorInput(float delta_time);

	const double getCursorX();
	const double getCursorY();
	const glm::mat4 getViewMat();

private:
	GLFWwindow* window;

	float camera_velocity;
	float camera_yaw;
	float camera_pitch;
	float camera_fov;

	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;

	float sight_velocity;
	double cursor_pos_x;
	double cursor_pos_y;

	const glm::vec3 calCameraFront();
};