#include "Component/LightComponent.h"

#include "Game.h"
#include "RenderManager.h"
#include "LightManager.h"

COMPONENT_DEFINITION(Component, LightComponent);

LightComponent::LightComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	is_active = false;
}

// --------------
// there is a template function for initialize light in "LightComponent.h"
// --------------


void LightComponent::update(float delta)
{
	// assign light to render manager
	Game::access().render_manager->light_manager->assign_light(getComponent<LightComponent>());
}