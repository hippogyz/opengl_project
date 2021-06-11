#include "Component/LightComponent.h"

#include "Game.h"
#include "GameObject.h"
#include "Component/TransformComponent.h"
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

void LightComponent::update_trans_information()
{
	light->position = gameobject->transform->g_position;
	light->direction = glm::mat3_cast(gameobject->transform->g_rotation) * glm::vec3(1.0f, 0.0f, 0.0f);
}

void LightComponent::update(float delta)
{
	// assign light to render manager
	Game::access().render_manager->light_manager->assign_light(getComponent<LightComponent>());
}