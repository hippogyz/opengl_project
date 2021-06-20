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
	render_order = NORMAL_RENDER_ORDER;

	pre_write_mask = 0x00;
	pre_stencil_mask = 0xff;
	pre_stencil_ref = 0xff;
	pre_stencil_func = GL_ALWAYS;
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
		case NORMAL_RENDER_ORDER:
			render_manager->normal_objects.push_back(gameobject);
			break;
		case SPECIAL_RENDER_ORDER:
			render_manager->special_objects.push_back(gameobject);
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
	glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint*)&pre_write_mask);
	glGetIntegerv(GL_STENCIL_FUNC, (GLint*)&pre_stencil_func);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint*)&pre_stencil_mask);
	glGetIntegerv(GL_STENCIL_REF, (GLint*)&pre_stencil_ref);

	glStencilMask(write_mask);
	glStencilFunc(stencil_func, stencil_ref, stencil_mask);
}

void StencilRenderComponent::post_set_stencil()
{
	glStencilMask(pre_write_mask);
	glStencilFunc(pre_stencil_func, pre_stencil_ref, pre_stencil_mask);
	//glStencilMask(0x00);
	//glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

void StencilRenderComponent::default_stencil_set()
{
	set_write_mask(0x01);
	set_stencil_mask(0x01);
	set_stencil_ref(0x01);
	set_stencil_func(GL_ALWAYS);
	set_render_array(NORMAL_RENDER_ORDER);
}

void StencilRenderComponent::default_effect_stencil_set()
{
	set_write_mask(0x00); // prohibit writing to stencil buffer
	set_stencil_mask(0x01);
	set_stencil_ref(0x01);
	set_stencil_func(GL_NOTEQUAL);
	set_render_array(SPECIAL_RENDER_ORDER);
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


