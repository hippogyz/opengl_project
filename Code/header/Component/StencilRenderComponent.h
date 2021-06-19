#pragma once

#include "Component/RenderComponent.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum RenderArray { NORMAL = 1, SPECIAL = 2 };

class StencilRenderComponent : public RenderComponent
{
	COMPONENT_DECLARATION(StencilRenderComponent);

public:
	StencilRenderComponent(GameObject* gameobject, int order = RENDER_ORDER);
	virtual ~StencilRenderComponent();

	virtual void assign_renderer_for_each_frame();

	void set_write_mask(unsigned int mask);
	void set_stencil_mask(unsigned int mask);
	void set_stencil_ref(unsigned int ref);
	void set_stencil_func(GLenum func);
	void set_render_array(RenderArray render_order);

protected:
	virtual void update(float delta);

	void pre_set_stencil();
	void post_set_stencil();

protected:
	unsigned int write_mask;
	unsigned int stencil_mask;
	unsigned int stencil_ref; // most of time is 0xFF, use stencil_mask for more control 
	GLenum stencil_func;
	RenderArray render_order;

	unsigned int pre_write_mask;
	unsigned int pre_stencil_mask;
	unsigned int pre_stencil_ref;
	GLenum pre_stencil_func;
};