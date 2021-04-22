#include "Component/RenderComponent.h"
#include <iostream>
#include "Game.h"
#include "RenderManager.h"

COMPONENT_DEFINITION(Component, RenderComponent);

RenderComponent::RenderComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	is_active = false;
}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::initialize_renderer(std::string model_path, const char* vs_path, const char* fs_path)
{
	RenderManager* render_manager = Game::access().render_manager.get();
	model = render_manager->assign_model(model_path);
	shader = render_manager->assign_shader_VF(vs_path, fs_path);
	is_active = true;
}

// for RenderComponent, update is equal to render
void RenderComponent::update(float delta) 
{
	//////////////////////////
}
