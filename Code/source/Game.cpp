#include "Game.h"
#include "GameObject.h"
#include "RenderManager.h"
#include "InputManager.h"

#include <iostream>

// for test
#include "TestObject/Cubic.h"
#include "TestObject/PointLightObject.h"
#include "TestObject/SpotLightObject.h"
#include "TestObject/CubicMoveComponent.h"
#include "TestObject/ChosenEffect.h"
#include "Component/TransformComponent.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
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

void Game::execute()
{
	initialize();

	float real_time = glfwGetTime();

	while (!exit_window)
	{
		float pre_time = real_time;
		real_time = glfwGetTime();
		
		float delta_time = (real_time - pre_time < 0.05f) ? real_time - pre_time : 0.05f;

		game_update(delta_time);
	}

	uninitialize();
}

void Game::initialize()
{
	std::shared_ptr<GameObject> obj;
	std::shared_ptr<GameObject> obj2;

	obj = add_object<Cubic>(glm::vec3(0.0, -1.0, -3.0), "Cubic(0)");
	//obj->initialize_object();

	obj = add_object<Cubic>(glm::vec3(0.0, 0.5, -5.0), "Cubic(1)");
	//obj->initialize_object();

	obj = add_object<Cubic>(glm::vec3(1.0, -0.5, -2.0), "Cubic(2)");
	//obj->initialize_object();
	obj->addComponent<CubicMoveComponent>(obj.get());

	obj2 = add_object<ChosenEffect>(obj.get());
	//obj2->initialize_object();

	obj = add_object<GameObject>("temp_camera");
	//obj->initialize_object();
	obj->transform->set_local_position(glm::vec3(-10.0, 0.0, -3.0));
	obj->addComponent<CameraComponent>(obj.get());
	obj->addComponent<FPSControlComponent>(obj.get());

	obj = add_object<PointLightObject>(glm::vec3(-3.0f, -2.0f, -5.0f),"test_point_light");
	//obj->initialize_object();

	obj = add_object<SpotLightObject>(glm::vec3(5.0f), glm::vec3(-1.0f), "test_spot_light");
	//obj->initialize_object();

	obj = add_object<GameObject>("dir_light");
	//obj->initialize_object();
	std::shared_ptr<LightComponent> light = obj->addComponent<LightComponent>(obj.get());
	light->initializeLight<DirLight>(true, glm::vec3(0.6, 0.6, 0.0), glm::vec3(1.0), glm::vec3(0.0f));
	obj->transform->set_local_rotation(glm::vec3(0.0, 0.0, 1.0), 90);

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
		if (object->is_active)
		{
			 object -> uniform_update(delta);
		}
	}
}

void Game::physics_update(float delta)
{
	for (auto&& object : object_list)
	{
		if (object->is_active)
		{
			object -> physics_update(delta);
		}
	}
}

void Game::render(float delta)
{
	// call openGL
	render_manager->DoRender(delta);
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