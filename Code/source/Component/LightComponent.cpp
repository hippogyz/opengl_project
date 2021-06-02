#include "Component/LightComponent.h"

#include "Game.h"
#include "RenderManager.h"

COMPONENT_DEFINITION(Component, LightComponent);

LightComponent::LightComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
//	is_active = false;

	//for test
	glm::vec3 spot_light_position = glm::vec3(3.0);
	glm::vec3 spot_light_direction = glm::vec3(-1.0);
	initializeLight<SpotLight>(true, glm::vec3(0.7), glm::vec3(1.0), spot_light_position, spot_light_direction);
}

void LightComponent::update(float delta)
{
	// assign light to render manager
	Game::access().render_manager->lights.push_back( getComponent<LightComponent>() );
}