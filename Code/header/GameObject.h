#pragma once

#include <vector>
#include <memory>

class Component;

class GameObject {
public: // member
	std::vector< std::unique_ptr<Component> > component_list;
	bool is_active;

public: // method
	GameObject(bool active);
	virtual ~GameObject();

	void uniform_update(float delta);
	
	template < typename ComponentName > 
		ComponentName* getComponent();
	template < typename ComponentName, typename... Args >
		void addComponent(Args&&... args);
	template < typename ComponentName >
		void removeComponent();

		void test_printComponentName();
		void test_printAddList();
		void test_printRemoveList();
		void test_arrange();

protected:
	virtual void start();
	virtual void update(float delta);

private: // member
	bool first_update;
	std::vector< unsigned int > remove_buffer;
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