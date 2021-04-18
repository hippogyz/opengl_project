#pragma once
#include <list>
#include <memory>

class GameObject;

class Game {
public:
	void initialize();
	void game_update(float delta);
	void add_object(std::unique_ptr<GameObject>&& gameobject);

private:
	std::list < std::unique_ptr<GameObject> >object_list;
	std::list< std::unique_ptr<GameObject> >add_buffer;

private:
	void process_input(float delta);
	void uniform_update(float delta);
	void physics_update(float delta);
	void render(float delta);
	void arrange_object_list();
};