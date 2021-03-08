#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern const float VELOCITY;
extern const float SIGHT_VELOCITY;
extern const float YAW;
extern const float PITCH;
extern const float FOV;

enum MoveDirection { MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT };

class Camera {
public:
	Camera(const glm::vec3 & ini_pos = glm::vec3(0.0f, 0.0f, 3.0f),
		float ini_yaw = YAW, float ini_pitch = PITCH,
		const glm::vec3 & ini_up = glm::vec3(0.0f, 1.0f, 0.0f));

	Camera(float pos_x, float pos_y, float pos_z,
		float ini_yaw = YAW, float ini_pitch = PITCH,
		float up_x = 0.0f, float up_y = 1.0f, float up_z = 0.0f);
		
	void moveCamera(float deltaTime, MoveDirection dir);
	void rotateCamera(float deltaTime, float cursor_dx, float cursor_dy);

	const glm::vec3 getPosition();
	const glm::mat4 getViewMat();

protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float velocity;
	float sight_velocity;
	float yaw;
	float pitch;
	float fov;

	void updateCameraVector();
};