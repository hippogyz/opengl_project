#include "Game.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "InputManager.h"

#include <iostream>

// for test
#include "TestObject/Cubic.h"
#include "TestObject/CubicMoveComponent.h"
#include "Component/TransformComponent.h"
#include "Model.h"
#include "Shader.h"

Game& Game::access()
{
	static Game game;
	return game;
}

Game::Game()
{
	render_manager = std::make_shared<RenderManager>();
	input_manager = std::make_shared<InputManager>(render_manager->window);
}

Game::~Game()
{

}

void Game::initialize()
{
	add_object<Cubic>("Cubic(0)");
	add_object<Cubic>("Cubic(1)");
	add_object<Cubic>("Cubic(2)");

	arrange_object_list();

	auto obj = object_list.begin();
	(*obj)->transform->set_local_position(glm::vec3(0.0, -1.0, -3.0));
	obj++;
	(*obj)->transform->set_local_position(glm::vec3(0.0, 0.5, -5.0));
	obj++;
	(*obj)->transform->set_local_position(glm::vec3(1.0, -0.5, -2.0));
	(*obj)->addComponent<CubicMoveComponent>( obj->get() );
}

void Game::game_update(float delta)
{
	process_input(delta);
	uniform_update(delta);
	physics_update(delta);
	render(delta);

	arrange_object_list();
}


void Game::process_input(float delta)
{
	input_manager->process_input(delta);
}

void Game::uniform_update(float delta)
{
	for (auto&& object : object_list)
	{
		if (object->is_alive)
		{
			 object -> uniform_update(delta);
		}
	}
}

void Game::physics_update(float delta)
{
	for (auto&& object : object_list)
	{
		if (object->is_alive)
		{
			object -> physics_update(delta);
		}
	}
}

void Game::render(float delta)
{
	// call openGL
	render_manager->BeforeRender(delta);
	
	for (auto&& object : object_list)
	{
		if (object->is_alive)
		{
			object->render(delta); // with draw call
		}
	}
	
	render_manager->AfterRender(delta);
}

void Game::arrange_object_list()
{
	for (auto it = object_list.begin(); it != object_list.end(); )
	{
		if (!(*it)->is_alive) 
		{
			it = object_list.erase(it); 
		}
		else 
		{ 
			++it; 
		}
	}

	for (auto it = add_buffer.begin(); it != add_buffer.end(); ++it)
	{
		object_list.push_back(*it);
	}
	add_buffer.clear();
}