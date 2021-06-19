#pragma once

#include "Component.h"

class RenderManager;
class Shader;
class Model;
struct Vertex;
struct Texture;

class RenderComponent:public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	std::weak_ptr<Model> model;
	std::weak_ptr<Shader> shader;

public:
	RenderComponent(GameObject* gameobject, int order = RENDER_ORDER);
	//RenderComponent( GameObject* gameobject, std::string model_path, const char* vs_path, const char* fs_path, int order = RENDER_ORDER );
	virtual ~RenderComponent();

	virtual void initialize_renderer(std::string model_name, std::vector< Vertex > vertices, std::vector< unsigned int > indices, 
											std::vector<Texture>textures, const char* vs_path, const char* fs_path);
	virtual void initialize_renderer(std::string model_path, const char* vs_path, const char* fs_path);

	virtual void assign_renderer_for_each_frame();

protected:
	virtual void update(float delta);	// for RenderComponent, update is equal to render
	
protected:
	std::shared_ptr< RenderManager > render_manager;
};