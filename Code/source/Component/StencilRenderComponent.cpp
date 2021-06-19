#include "Component/StencilRenderComponent.h"

#include <iostream>
#include "Game.h"
#include "RenderManager.h"
//#include "Model.h"
//#include "Shader.h"
//#include "Component/TransformComponent.h"

COMPONENT_DEFINITION(RenderComponent, StencilRenderComponent);

StencilRenderComponent::StencilRenderComponent(GameObject* gameobject, int order) : RenderComponent(gameobject, order)
{
	write_mask = 0xFF;
	stencil_mask = 0xFF;
	stencil_ref = 0xFF;
	stencil_func = GL_ALWAYS;
	render_order = NORMAL;
}

StencilRenderComponent::~StencilRenderComponent()
{

}

void StencilRenderComponent::assign_renderer_for_each_frame()
{
	if (!is_active)
		return;

	switch (render_order)
	{
		case NORMAL:
			render_manager->normal_objects.push_back(gameobject);
			break;
		case SPECIAL:
			render_manager->normal_objects.push_back(gameobject);
			break;
	}
}

void StencilRenderComponent::update(float delta)
{
	pre_set_stencil();
	RenderComponent::update(delta);
	post_set_stencil();
}

void StencilRenderComponent::pre_set_stencil()
{
	glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint*)&write_mask);
	glGetIntegerv(GL_STENCIL_FUNC, (GLint*)&pre_stencil_func);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint*)&stencil_mask);
	glGetIntegerv(GL_STENCIL_REF, (GLint*)&stencil_ref);

	glStencilMask(write_mask);
	glStencilFunc(stencil_func, stencil_ref, stencil_mask);
}

void StencilRenderComponent::post_set_stencil()
{
	glStencilMask(pre_write_mask);
	glStencilFunc(pre_stencil_func, pre_stencil_ref, pre_stencil_mask);
}

void StencilRenderComponent::set_write_mask(unsigned int mask)
{
	this->write_mask = mask;
}

void StencilRenderComponent::set_stencil_mask(unsigned int mask)
{
	this->stencil_mask = mask;
}

void StencilRenderComponent::set_stencil_ref(unsigned int ref)
{
	this->stencil_ref = ref;
}

void StencilRenderComponent::set_stencil_func(GLenum func)
{
	this->stencil_func = func;
}

void StencilRenderComponent::set_render_array(RenderArray order)
{
	this->render_order = order;
}


