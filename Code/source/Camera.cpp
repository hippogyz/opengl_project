#include "Camera.h"

const float VELOCITY = 5.0f;
const float SIGHT_VELOCITY = 20.0f;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 1.0f;

const float SIGHT_LIMIT = 10.0f;
const float MAX_PITCH = 85.0f;

template <typename T>
static const T clamp(T target, T min, T max)
{
	target = (target < max) ? target : max;
	target = (target > min) ? target : min;
	return target;
}

Camera::Camera(const glm::vec3& ini_pos, const float ini_yaw, const float ini_pitch, const glm::vec3& ini_up)
	: velocity(VELOCITY), sight_velocity(SIGHT_VELOCITY), fov(FOV), front(glm::vec3(0.0f)), right(glm::vec3(0.0f))
{
	position = ini_pos;
	up = ini_up;

	yaw = ini_yaw;
	pitch = ini_pitch;

	updateCameraVector();
}

Camera::Camera(const float pos_x, const float pos_y, const float pos_z,
	const float ini_yaw, const float ini_pitch,
	const float up_x, const float up_y, const float up_z)
	: velocity(VELOCITY), sight_velocity(SIGHT_VELOCITY), fov(FOV), front(glm::vec3(0.0f)), right(glm::vec3(0.0f))
{
	position = glm::vec3(pos_x, pos_y, pos_z);
	up = glm::vec3(up_x, up_y, up_z);

	yaw = ini_yaw;
	pitch = ini_pitch;

	updateCameraVector();
}

void Camera::moveCamera(float deltaTime, MoveDirection dir)
{
	switch (dir)
	{
	case MOVE_FORWARD:
		position += front * deltaTime * velocity;
		break;
	case MOVE_BACKWARD:
		position -= front * deltaTime * velocity;
		break;
	case MOVE_RIGHT:
		position += right * deltaTime * velocity;
		break;
	case MOVE_LEFT:
		position -= right * deltaTime * velocity;
		break;
	}
}

void Camera::rotateCamera(float deltaTime, float cursor_dx, float cursor_dy)
{
	cursor_dx = clamp(cursor_dx, -SIGHT_LIMIT, SIGHT_LIMIT);
	cursor_dy = clamp(cursor_dy, -SIGHT_LIMIT, SIGHT_LIMIT);

	pitch = clamp(pitch + cursor_dy * deltaTime * sight_velocity,
		-MAX_PITCH, MAX_PITCH);
	yaw += cursor_dx * deltaTime * sight_velocity;

	updateCameraVector();
}


const glm::vec3 Camera::getPosition()
{
	return position;
}

const glm::mat4 Camera::getViewMat()
{
	return glm::lookAt(position, position + front, up);
}


void Camera::updateCameraVector()
{
	front = glm::vec3(
		cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
		sin(glm::radians(pitch)),
		cos(glm::radians(pitch)) * sin(glm::radians(yaw))
	);
	right = glm::normalize(glm::cross(front, up));
}
