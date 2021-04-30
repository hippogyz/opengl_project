#pragma once
#include <list>
#include <memory>

class GameObject;
class RenderManager;

class Game {
public:
	std::shared_ptr<RenderManager> render_manager;

public:
	// instance
	static Game& access();
	~Game();
	Game(const Game&) = delete;
	Game& operator = (const Game&) = delete;

	void initialize();
	void game_update(float delta);	
	template < typename GameObjectType, typename... Args >
		void add_object(Args&&... args);

private:
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
void Game::add_object(Args&&... args)
{
	auto game_object = std::make_shared<GameObjectType>(std::forward<Args>(args)...);
	add_buffer.push_back( game_object );
}