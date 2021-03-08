#include "InputController.h"

template <typename T>
static T clamp(T target, T min, T max)
{
	target = (target < max) ? target : max;
	target = (target > min) ? target : min;
	return target;
}

InputController::InputController()
{	
	this->window = nullptr;

	camera_yaw = -90.0f;
	camera_pitch = 0.0f;
	camera_fov = 1.0f;

	camera_velocity = 5.0f;
	camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
	camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	sight_velocity = 20.0f;
	cursor_pos_x = 0.0;
	cursor_pos_y = 0.0;
}

void InputController::initialize(GLFWwindow* window)
{
	this->window = window;
	camera_front = calCameraFront();
	glfwGetCursorPos(window, &cursor_pos_x, &cursor_pos_y);
}

void InputController::update(float delta_time)
{
	delta_time = (delta_time < 0.05) ? delta_time : 0.05;
	cursorInput(delta_time);
	processInput(delta_time);
}

void  InputController::processInput(float delta_time)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_velocity * delta_time * glm::normalize(camera_front - camera_up * glm::dot(camera_front, camera_up));
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_velocity * delta_time * glm::normalize(camera_front - camera_up * glm::dot(camera_front, camera_up));

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += camera_velocity * delta_time * glm::normalize(glm::cross(camera_front, camera_up));

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= camera_velocity * delta_time * glm::normalize(glm::cross(camera_front, camera_up));
}

void  InputController::cursorInput(float delta_time)
{
	double pre_x = cursor_pos_x;
	double pre_y = cursor_pos_y;
	glfwGetCursorPos(window, &cursor_pos_x, &cursor_pos_y);

	float dx = clamp( float(cursor_pos_x - pre_x), -10.0f, 10.0f);
	float dy = clamp( float(pre_y - cursor_pos_y), -10.0f, 10.0f);

	camera_pitch += dy * delta_time * sight_velocity;
	camera_yaw += dx * delta_time * sight_velocity;

	camera_pitch = clamp(camera_pitch, -80.0f, 80.0f);

	camera_front = calCameraFront();
}

const double  InputController::getCursorX() { return cursor_pos_x; }
const double  InputController::getCursorY() { return cursor_pos_y; }

const glm::mat4 InputController::getViewMat()
{
	return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

const glm::vec3 InputController::calCameraFront()
{
	return glm::vec3(
		cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch)),
		sin(glm::radians(camera_pitch)),
		sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch))
	);
}

