#pragma once

#include <vector>
#include <memory>
#include "Component.h"

class GameObject {
public: // member
	std::vector< std::unique_ptr<Component> > component_list;
	bool is_active;

public: // method
	GameObject(bool active);
	virtual ~GameObject();

	void uniform_update(float delta);
	virtual void start();
	virtual void update(float delta);
	
	template < typename ComponentName > 
		ComponentName& getComponent();
	template < typename ComponentName, typename... Args >
		void addComponent(Args&&... args);
	template < typename ComponentName >
		void removeComponent();

		void test_printComponentName();
		void test_printAddList();
		void test_printRemoveList();
		void test_arrange();

private: // member
	bool first_update;
	std::vector< unsigned int > remove_buffer;
	std::vector< std::unique_ptr<Component> > add_buffer;
	
private: // method
	void arrangeComponent();
};


//   ---------- template method ----------------//
template < typename ComponentName >
ComponentName& GameObject::getComponent()
{
	for (auto&& component : component_list)
	{
		if (component->isComponentType(ComponentName::Type))
		{
			return *static_cast<ComponentName*> (component.get());
		}
	}

	return *std::unique_ptr<ComponentName>(nullptr);
}

template < typename ComponentName, typename... Args >
void GameObject::addComponent(Args&&... args)
{
	std::unique_ptr<ComponentName> component;
	component = std::unique_ptr<ComponentName>(
		new ComponentName(std::forward<Args>(args)...));

	auto it = add_buffer.begin();
	while (it != add_buffer.end() && component->order > (*it)->order)
	{
		++it;
	}

	add_buffer.insert(it, std::move(component));
}

template <typename ComponentName>
void GameObject::removeComponent()
{
	int index = 0;
	for (auto&& component : component_list)
	{
		if (component ->isComponentType(ComponentName::Type) && component ->remove_call() )
		{
			remove_buffer.push_back(index);
		}
		++index;
	}
}