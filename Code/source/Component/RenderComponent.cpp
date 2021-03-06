#include "Component/RenderComponent.h"

#include <iostream>
#include "Game.h"
#include "RenderManager.h"
#include "Model.h"
#include "Shader.h"
#include "Component/TransformComponent.h"

COMPONENT_DEFINITION(Component, RenderComponent);

RenderComponent::RenderComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	is_active = false;
	render_manager = Game::access().render_manager;
}

RenderComponent::~RenderComponent()
{

}

void RenderComponent::initialize_renderer(std::string model_name, std::vector< Vertex > vertices, std::vector< unsigned int > indices,
																			std::vector<Texture>textures, const char* vs_path, const char* fs_path)
{
//	RenderManager* render_manager = Game::access().render_manager.get();
	model = render_manager->assign_model(model_name, vertices, indices, textures );
	shader = render_manager->assign_shader_VF(vs_path, fs_path);
	is_active = true;
}

void RenderComponent::initialize_renderer(std::string model_path, const char* vs_path, const char* fs_path)
{
	//RenderManager* render_manager = Game::access().render_manager.get();
	model = render_manager->assign_model(model_path);
	shader = render_manager->assign_shader_VF(vs_path, fs_path);
	is_active = true;
}

void RenderComponent::initialize_renderer(std::weak_ptr<Model>& model, const char* vs_path, const char* fs_path)
{
	this->model = model;
	shader = render_manager->assign_shader_VF(vs_path, fs_path);
	is_active = true;
}

// for RenderComponent, update is equal to render
void RenderComponent::update(float delta) 
{
	std::shared_ptr<Shader> mShader = shader.lock();

	glm::mat4 model_trans = gameobject->transform->get_trans_matrix();
	glm::mat4 norm_mat = glm::transpose(glm::inverse(model_trans));

	mShader->useShader();
	mShader->setTrans("model", model_trans);
	mShader->setTrans("norm_mat", norm_mat);

	model.lock()->Draw(mShader.get());
}

void RenderComponent::assign_renderer_for_each_frame()
{
	if (!is_active)
		return;
	
	render_manager->normal_objects.push_back(gameobject);
}