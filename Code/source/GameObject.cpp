#include "GameObject.h"
#include "Component.h"

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

	arrangeComponent();
}

void GameObject::start()
{

}

void GameObject::update(float delta)
{

}

template < typename ComponentName >
ComponentName& GameObject::getComponent()
{
	for (auto&& component : component_list)
	{
		if (component.isComponentType(ComponentName::Type))
		{
			return *static_cast<ComponentName*> (component.get());
		}
	}

	return *std::unique_ptr<ComponentName> (nullptr);
}

template < typename ComponentName, typename... Args >
void GameObject::addComponent(Args&&... args)
{
	std::unique_ptr<ComponentName> component;
	component = std::unique_ptr<ComponentName>(
		new ComponentName(std::forward<Args>(args)...) );

	auto it = add_buffer.begin();
	while (		it != add_buffer.end() &&
				component->order > (*it)->order )
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
		if (component.isComponentType(ComponentName::Type))
		{
			remove_buffer.push_back(index);
		}
		++index;
	}
}

void GameObject::arrangeComponent()
{
	std::sort(remove_buffer.begin(), remove_buffer.end());
	for (auto it = remove_buffer.rbegin(); it != remove_buffer.rend(); ++it)
	{
		auto component = component_list.begin();
		component += *it;
		(*component).reset();
		component_list.erase(component);
	}
	remove_buffer.clear();

	auto component = component_list.begin();
	for (auto it = add_buffer.begin(); it != add_buffer.end(); ++it)
	{
		while ((*it)->order > (*component)->order && component != component_list.end())
		{
			++component;
		}

		component_list.insert(component, std::move(*it));
		++component;
	}
}