#include "TestObject/CubicComponent.h"

#include "Component/TransformComponent.h"

COMPONENT_DEFINITION(Component, CubicComponent);

CubicComponent::CubicComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{

}

void CubicComponent::update(float delta)
{
	gameobject->transform->local_rotate(glm::vec3(1.0), delta);
}