#include "Game.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "InputManager.h"

#include <iostream>

// for test
#include "TestObject/Cubic.h"
#include "TestObject/CubicMoveComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"
#include "Component/FPSControlComponent.h"
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

	exit_window = false;
}

Game::~Game()
{

}

void Game::initialize()
{
	add_object<Cubic>("Cubic(0)");
	add_object<Cubic>("Cubic(1)");
	add_object<Cubic>("Cubic(2)");
	add_object<GameObject>("temp_camera");

	arrange_object_list();

	auto obj = object_list.begin();
	(*obj)->transform->set_local_position(glm::vec3(0.0, -1.0, -3.0));
	obj++;
	(*obj)->transform->set_local_position(glm::vec3(0.0, 0.5, -5.0));
	obj++;
	(*obj)->transform->set_local_position(glm::vec3(1.0, -0.5, -2.0));
	(*obj)->addComponent<CubicMoveComponent>( obj->get() );
	obj++;
	//(*obj)->addComponent<CubicMoveComponent>(obj->get());
	(*obj)->transform->set_local_position(glm::vec3(-10.0, 0.0, -3.0));
	(*obj)->addComponent<CameraComponent>(obj->get());
	(*obj)->addComponent<FPSControlComponent>(obj->get());
}

void Game::uninitialize()
{
	object_list.clear();
	input_manager.reset();
	render_manager.reset();
}

bool Game::game_update(float delta)
{
	process_input(delta);
	uniform_update(delta);
	physics_update(delta);
	render(delta);

	arrange_object_list();

	return exit_window;
}


void Game::process_input(float delta)
{
	exit_window = input_manager->exit_window();
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