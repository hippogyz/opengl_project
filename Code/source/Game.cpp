#include "Game.h"
#include "GameObject.h"

void Game::initialize()
{

}

void Game::game_update(float delta)
{
	// process input

	uniform_update(delta);
	physics_update(delta);
	// render

	arrange_object_list();
}

void Game::add_object(std::unique_ptr<GameObject>&& object)
{
	add_buffer.emplace_back(std::move(object));
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
}

void Game::arrange_object_list()
{
	for (auto it = object_list.begin(); it != object_list.end(); )
	{
		if (!(*it)->is_alive) 
		{
			(*it).reset();
			it = object_list.erase(it); 
		}
		else 
		{ 
			++it; 
		}
	}

	for (auto it = add_buffer.begin(); it != add_buffer.end(); ++it)
	{
		object_list.emplace_back(std::move(*it));
	}
	add_buffer.clear();
}