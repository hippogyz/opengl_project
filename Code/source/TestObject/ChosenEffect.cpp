#include "TestObject/ChosenEffect.h"

#include "Model.h"
#include "Shader.h"

#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/StencilRenderComponent.h"

ChosenEffect::ChosenEffect(GameObject* parent, std::string name, bool active) : GameObject(name, active), parent(parent)
{
	transform->set_parent(parent);
	transform->set_local_position(glm::vec3(0.0f));
	transform->set_local_rotation( glm::quat_cast(glm::mat3{ { 1.0, 0.0, 0.0 }, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} }) );
	transform->set_local_scale(1.1f);
}

void ChosenEffect::initialize_object()
{
	is_active = true;
	initialize_chosen_effect();
}

void ChosenEffect::update(float delta)
{
	// check parent
	// if fails, destroy this object
	if (transform->parent_trans.lock()->gameobject != parent)
	{
		is_active = false;
		is_alive = false;
	}
}

void ChosenEffect::initialize_chosen_effect()
{
	static const char* vs_path = "opengl_project/Code/shader/vertex_shader.vs";
	static const char* fs_path = "opengl_project/Code/shader/ref_frag_shader.fs";

	auto stencil_renderer = std::make_shared<StencilRenderComponent>(this);
	stencil_renderer->default_effect_stencil_set();

	renderer = stencil_renderer;
	//	renderer = std::make_shared<RenderComponent>(this);
	renderer->initialize_renderer(parent->renderer->model, vs_path, fs_path);
	std::cout << "chosen effect builded " << std::endl;
}