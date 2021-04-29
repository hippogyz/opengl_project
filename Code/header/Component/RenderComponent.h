#pragma once

#include "Component.h"

class Shader;
class Model;

class RenderComponent:public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	std::weak_ptr<Model> model;
	std::weak_ptr<Shader> shader;

public:
	RenderComponent(GameObject* gameobject, int order = RENDER_ORDER);
	//RenderComponent( GameObject* gameobject, std::string model_path, const char* vs_path, const char* fs_path, int order = RENDER_ORDER );
	virtual ~RenderComponent();

	void initialize_renderer(const Model& t_model, const char* vs_path, const char* fs_path);
	void initialize_renderer(std::string model_path, const char* vs_path, const char* fs_path);

protected:
	virtual void update(float delta);	// for RenderComponent, update is equal to render
};