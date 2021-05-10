#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraComponent : public Component
{
	COMPONENT_DECLARATION(CameraComponent)

public:
	CameraComponent(GameObject* gameobject, int order = 5);
	virtual ~CameraComponent();

	glm::vec3 get_position();
	virtual glm::mat4 get_view();

protected:
	virtual void start();
};