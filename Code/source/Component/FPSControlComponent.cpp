#include "Component/FPSControlComponent.h"

#include "Game.h"
#include "GameObject.h"
#include "Component/TransformComponent.h"
#include "InputManager.h"

COMPONENT_DEFINITION(Component, FPSControlComponent);

FPSControlComponent::FPSControlComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	move_velocity = 0.05f;
	sight_velocity = 0.20f;
	pitch_limit = 80.0f;

	sight_front = glm::vec3(1.0, 0.0, 0.0);
	front = glm::vec3(1.0, 0.0, 0.0);
	right = glm::vec3(0.0, 0.0, 1.0);
	up = glm::vec3(0.0, 1.0, 0.0);
}

void FPSControlComponent::start()
{
	input_manager = Game::access().input_manager;
}

void FPSControlComponent::update(float delta)
{
	update_vectors();

	glm::vec3 move_dir = get_move_dir();
	move(delta, move_dir);
	rotate_sight(delta);
}

void FPSControlComponent::update_vectors()
{
	glm::mat3 rotation = glm::mat3_cast(gameobject->transform->rotation);
	
	sight_front = rotation * glm::vec3(1.0, 0.0, 0.0);
	if (sight_front.y != 1.0 && sight_front.y != -1.0)
		right = glm::normalize(glm::cross(sight_front, up));
	front = glm::cross( up, right );

	// yaw pitch
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


}