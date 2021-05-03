#include "TestObject/CubicMoveComponent.h"
#include "Component/TransformComponent.h"

#include "Game.h"
#include "InputManager.h"

COMPONENT_DEFINITION(Component, CubicMoveComponent);

CubicMoveComponent::CubicMoveComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	velocity = 0.05f;
}

void CubicMoveComponent::start()
{
	input_manager = Game::access().input_manager;
}

void CubicMoveComponent::update(float delta)
{
	glm::vec3 dir = get_direction();

	gameobject->transform->local_translate(dir * velocity * delta);
}

glm::vec3 CubicMoveComponent::get_direction()
{
	float x = 0.0f; 
	float y = 0.0f;
	float z = 0.0f;
	if (input_manager->is_hold("D"))
		x += 1.0f;
	if (input_manager->is_hold("A"))
		x -= 1.0f;
	if (input_manager->is_hold("S"))
		z += 1.0f;
	if (input_manager->is_hold("W"))
		z -= 1.0f;

	return glm::vec3(x, y, z);
}
