#include "GameObject.h"
#include "Component.h"

#include <iostream>
#include <algorithm> // std::sort

GameObject::GameObject(bool active) : is_active(active)
{
	first_update = false;
}

GameObject::~GameObject()
{

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
	std::sort(remove_buffer.begin(), remove_buffer.end());
	for (auto it = remove_buffer.rbegin(); it != remove_buffer.rend(); ++it)
	{
		auto it_com = component_list.begin();
		it_com += *it;
		(*it_com).reset();
		component_list.erase(it_com);
	}
	remove_buffer.clear();

	int index_com = 0;
	std::sort(add_buffer.begin(), add_buffer.end(), compare_component_order);
	for (auto it = add_buffer.begin(); it != add_buffer.end(); ++it)
	{
		auto it_com = component_list.begin() + index_com;
		while (it_com != component_list.end() && (*it)->order > (*it_com)->order)
		{
			++it_com;
			++index_com;
		}

		component_list.insert(it_com, std::move(*it));
		++index_com;
	}
	add_buffer.clear();
}


// ----------------- method for test ----------------- //
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

void GameObject::test_printRemoveList()
{
	std::cout << "remove list: ";
	for (auto it = remove_buffer.begin(); it != remove_buffer.end(); ++it)
	{
		std::cout << '['<< *it << ']' << component_list[ *it ]->getComponentType() << "  ";
	}
	std::cout << std::endl;
}

void GameObject::test_arrange()
{
	arrangeComponent();
}