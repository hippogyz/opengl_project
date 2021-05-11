#include "Component/FPSControlComponent.h"

#include "Game.h"
#include "GameObject.h"
#include "Component/TransformComponent.h"
#include "InputManager.h"

#include <iostream>

COMPONENT_DEFINITION(Component, FPSControlComponent);

FPSControlComponent::FPSControlComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	move_velocity = 5.0f;
	sight_velocity = 30.0f;
	pitch_limit = 80.0f;

	pitch = 0.0f;
	yaw = 0.0f;

	sight_front = glm::vec3(1.0, 0.0, 0.0);
	front = glm::vec3(1.0, 0.0, 0.0);
	right = glm::vec3(0.0, 0.0, 1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
}

void FPSControlComponent::start()
{
	input_manager = Game::access().input_manager;
	update_vectors();

	input_manager->register_key("P", GLFW_KEY_P);
}

void FPSControlComponent::update(float delta)
{
	glm::vec3 move_dir = get_move_dir();
	move(delta, move_dir);
	rotate_sight(delta);

	update_vectors();
}

void FPSControlComponent::update_vectors()
{
	float rp = glm::radians(pitch);
	float ry = glm::radians(yaw);

	sight_front = glm::vec3(cos(rp) * cos(ry), sin(rp), -cos(rp) * sin(ry));
	right = glm::normalize(glm::cross(sight_front, up));
	front = glm::cross(up, right);

	gameobject->transform->set_local_rotation(pitch, yaw, 0.0f);

	if (input_manager->is_press("P"))
	{
		glm::vec3 posi = gameobject->transform->g_position;
		std::cout << std::endl;
		std::cout<< "position: "<< posi.x << ", " << posi.y << ", " << posi.z << std::endl;
		std::cout << "sight: " << sight_front.x << ", " << sight_front.y << ", " << sight_front.z << std::endl;
	}

//	glm::mat3 rotation = glm::mat3_cast(gameobject->transform->rotation);
	
//	sight_front = rotation * glm::vec3(1.0, 0.0, 0.0);
//	if (sight_front.y != 1.0 && sight_front.y != -1.0)
//		right = glm::normalize(glm::cross(sight_front, up));
//	front = glm::cross( up, right );
}

glm::vec3 FPSControlComponent::get_move_dir()
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	if (input_manager->is_hold("D"))
		z += 1.0f;
	if (input_manager->is_hold("A"))
		z -= 1.0f;
	if (input_manager->is_hold("W"))
		x += 1.0f;
	if (input_manager->is_hold("S"))
		x -= 1.0f;

	return glm::vec3(x, y, z);
}

void FPSControlComponent::move(float delta, glm::vec3& dir)
{
	glm::vec3 trans = (front * dir.x + up * dir.y + right * dir.z) * delta * move_velocity;
	gameobject->transform->local_translate(trans);
}

void FPSControlComponent::rotate_sight(float delta)
{
	float* cursor_move = input_manager->get_cursor_move();

	yaw = yaw + -cursor_move[0] * delta * sight_velocity;
	pitch = glm::clamp(pitch + -cursor_move[1] * delta * sight_velocity, -pitch_limit, pitch_limit);

	if (yaw > 180.0)
	{
		yaw -= 360;
	}
	if (yaw < -180.0)
	{
		yaw += 360;
	}
}