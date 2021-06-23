#pragma once
#include <list>
#include <memory>

class GameObject;
class RenderManager;
class InputManager;

class Game {
public:
	std::shared_ptr<RenderManager> render_manager;
	std::shared_ptr<InputManager> input_manager;

public:
	// instance
	static Game& access();
	~Game();
	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;

	void execute();

	void initialize();
	void uninitialize();
	bool game_update(float delta);	
	template < typename GameObjectType, typename... Args >
		std::shared_ptr<GameObject> add_object(Args&&... args);

private:
	bool exit_window;
	
	std::list < std::shared_ptr<GameObject> >object_list;
	std::list< std::shared_ptr<GameObject> >add_buffer;

private:
	Game();

	void process_input(float delta);
	void uniform_update(float delta);
	void physics_update(float delta);
	void render(float delta);
	void arrange_object_list();
};

template < typename GameObjectType, typename... Args >
std::shared_ptr<GameObject> Game::add_object(Args&&... args)
{
	auto game_object = std::make_shared<GameObjectType>(std::forward<Args>(args)...);
	game_object->initialize_object();
	add_buffer.push_back( game_object );
	return game_object;
}