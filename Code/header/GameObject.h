#pragma once

#include <vector>
#include <memory>

class Component;
class TransformComponent;

class GameObject {
public: // member
	bool is_alive; // for Game class

	bool is_active;
	std::vector< std::unique_ptr<Component> > component_list;
	std::unique_ptr<TransformComponent> transform;

public: // method
	GameObject(bool active = true);
	virtual ~GameObject();

	//void process_input(float delta);
	void uniform_update(float delta);
	void physics_update(float delta);
	//void render();
	
	template < typename ComponentName > 
		ComponentName* getComponent();
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
	std::vector< std::unique_ptr<Component> > add_buffer;
	
private: // method
	void arrangeComponent();
};




//   ---------- template method ---------- //
template < typename ComponentName >
ComponentName* GameObject::getComponent()
{
	for (auto&& component : component_list)
	{
		if (component->isComponentType(ComponentName::Type))
		{
			return static_cast<ComponentName*> (component.get());
		}
	}

	return nullptr;
}

template < typename ComponentName, typename... Args >
void GameObject::addComponent(Args&&... args)
{
	auto component = std::unique_ptr<ComponentName>(new ComponentName(std::forward<Args>(args)...));
	add_buffer.push_back(std::move(component));
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