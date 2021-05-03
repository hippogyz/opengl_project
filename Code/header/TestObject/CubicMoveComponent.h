#pragma once

#include "Component.h"
#include <memory>
#include <glm/glm.hpp>

class InputManager;

class CubicMoveComponent : public Component
{
	COMPONENT_DECLARATION(CubicMoveComponent)

public:
	CubicMoveComponent(GameObject* gameobject, int order = 2);

protected:
	std::shared_ptr<InputManager> input_manager;
	float velocity;

	virtual void start();
	virtual void update(float delta);

	glm::vec3 get_direction();
};