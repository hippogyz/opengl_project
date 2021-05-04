#pragma once
#include "Component.h"
#include "InputManager.h"

#include <memory>


class CameraComponent : public Component
{
	COMPONENT_DECLARATION(CameraComponent)

public:
	CameraComponent(GameObject* gameobject, int order = 5);

protected:
	std::shared_ptr<InputManager> input_manager;
	float camera_velocity;
	float sight_velocity;

	virtual void start();
	virtual void update(float delta);
};