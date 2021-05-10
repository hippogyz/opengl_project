#pragma once

#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

class InputManager;

// front: x+-axis; right: z+-axis; up: y+-axis
class FPSControlComponent : public Component
{
	COMPONENT_DECLARATION(FPSControlComponent);

public:
	FPSControlComponent(GameObject* gameobject, int order = 4);
	virtual ~FPSControlComponent() {};

protected:
	std::shared_ptr<InputManager> input_manager;

	float move_velocity;
	float sight_velocity;
	float pitch_limit;

	glm::vec3 sight_front;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	virtual void start();
	virtual void update(float delta);

	void update_vectors();
	glm::vec3 get_move_dir();
	void move(float delta, glm::vec3 & dir);
	void rotate_sight(float delta);
	
};