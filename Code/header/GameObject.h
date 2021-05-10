#pragma once

#include <vector>
#include <memory>
#include <string>

class Component;
class TransformComponent;
class RenderComponent;

class GameObject {
public: // member
	bool is_alive; // for Game class
	std::size_t object_hash;

	bool is_active;
	std::vector< std::shared_ptr<Component> > component_list;
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;

public: // method
	GameObject(std::string name, bool active = true);
	virtual ~GameObject();

	//void process_input(float delta);
	void uniform_update(float delta);
	void physics_update(float delta);
	void render(float delta);

	void rename(std::string name);
	
	template < typename ComponentName > 
		std::weak_ptr<ComponentName> getComponent();
	template < typename ComponentName, typename... Args >
		void addComponent(Args&&... args);
	template < typename ComponentName >
		void removeComponent();

		void test_gameObject();
		void test_printComponentName();
		void test_printAddList();
		void test_arrange();

protected:
	virtual void start();
	virtual void update(float delta);

private: // member
	bool first_update;
	std::vector< std::shared_ptr<Component> > add_buffer;
	
private: // method
	void arrangeComponent();
};




//   ---------- template method ---------- //
template < typename ComponentName >
std::weak_ptr<ComponentName> GameObject::getComponent()
{
	std::weak_ptr<ComponentName> wp;

	for (auto&& component : component_list)
	{
		if (component->isComponentType(ComponentName::Type))
		{
			wp = std::dynamic_pointer_cast<ComponentName>(component);
		}
	}

	return wp;
}

template < typename ComponentName, typename... Args >
void GameObject::addComponent(Args&&... args)
{
	auto component = std::make_shared<ComponentName>(std::forward<Args>(args)...);
	add_buffer.push_back( component );
}

template <typename ComponentName>
void GameObject::removeComponent()
{
	for (auto&& component : component_list)
	{
		if (component ->isComponentType(ComponentName::Type))
		{
			component->remove_call();
		}
	}
}