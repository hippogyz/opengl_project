#include "Component/LightComponent.h"

COMPONENT_DEFINITION(Component, LightComponent);

LightComponent::LightComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	is_active = false;
}

void LightComponent::update(float delta)
{
	// assign light to render manager
}