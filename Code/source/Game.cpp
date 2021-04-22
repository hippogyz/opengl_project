#include "Game.h"
#include "GameObject.h"
#include "RenderManager.h"

Game& Game::access()
{
	static Game game;
	return game;
}

Game::Game()
{
	initialize();
}

Game::~Game()
{

}

void Game::initialize()
{
	render_manager = std::make_shared<RenderManager>();
}

void Game::game_update(float delta)
{
	// process input

	uniform_update(delta);
	physics_update(delta);
	// render

	arrange_object_list();
}


void Game::process_input(float delta)
{
	for (auto&& object : object_list)
	{
		if (object->is_alive)
		{
			// object -> process_input(delta);
		}
	}
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
		// object->render ( object->renderer->uniform_update )
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