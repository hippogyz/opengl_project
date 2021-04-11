#include "GameObject.h"
#include "Component.h"
#include "Component/TransformComponent.h"

#include <iostream>
#include <algorithm> // std::sort

GameObject::GameObject(bool active) : is_active(active)
{
	is_alive = true;
	first_update = false;

	transform = std::unique_ptr<TransformComponent>( new TransformComponent(this) );
}

GameObject::~GameObject()
{
	transform.reset();

	for (auto&& component : component_list)
	{
		component.reset();
	}
	component_list.clear();

	for (auto&& component : add_buffer)
	{
		component.reset();
	}
	add_buffer.clear();
}

void GameObject::uniform_update(float delta)
{
	arrangeComponent();

	if (!is_active) return;

	if (first_update)
	{
		start();
		first_update = false;
	}
	for (auto && component : component_list)
	{
		component->uniform_update(delta);
	}
	update(delta);
}

void GameObject::physics_update(float delta)
{
	if ( is_active && transform )
	{
		transform->uniform_update(delta);
	}
}

void GameObject::start()
{

}

void GameObject::update(float delta)
{

}

const bool static compare_component_order(const std::unique_ptr<Component>& a, const std::unique_ptr<Component>& b)
{
	return a->order < b->order;
}

void GameObject::arrangeComponent()
{
	for (auto it = component_list.begin(); it != component_list.end(); )
	{
		if ((*it)->is_removed())
		{
			(*it).reset();
			it = component_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::sort(add_buffer.begin(), add_buffer.end(), compare_component_order);

	auto it_com = component_list.begin();
	auto it_add = add_buffer.begin();
	for ( ; it_add != add_buffer.end(); ++it_add)
	{
		while (it_com != component_list.end() && (*it_add)->order > (*it_com)->order)
		{
			++it_com;
		}

		it_com = component_list.insert(it_com, std::move(*it_add));
		++it_com;
	}
	add_buffer.clear();
}


// ----------------- method for test ----------------- //
void GameObject::test_gameObject()
{
	addComponent<TransformComponent>(this);
	addComponent<Component>(this, 5);
	addComponent<Component>(this, 15);
	test_printComponentName();
	test_printAddList();
	std::cout << std::endl << "arrange component list" << std::endl;
	test_arrange();
	test_printComponentName();
	test_printAddList();
	removeComponent<TransformComponent>();
	std::cout << std::endl << "remove transform" << std::endl;
	test_arrange();
	test_printComponentName();
	test_printAddList();
	removeComponent<TransformComponent>();
	removeComponent<Component>();
	std::cout << std::endl << "remove component" << std::endl;
	test_arrange();
	test_printComponentName();
	test_printAddList();
}

void GameObject::test_printComponentName()
{
	std::cout << "component list: ";
	for (auto it = component_list.begin(); it != component_list.end(); ++it)
	{
		std::cout<< '('<< (*it)->order <<')' << (*it)->getComponentType() << "  ";
	}
	std::cout << std::endl;
}

void GameObject::test_printAddList()
{
	std::cout << "add list: " ;
	for (auto it = add_buffer.begin(); it != add_buffer.end(); ++it)
	{
		std::cout << '(' << (*it)->order << ')' << (*it)->getComponentType() << "  ";
	}
	std::cout << std::endl;
}

void GameObject::test_arrange()
{
	arrangeComponent();
}