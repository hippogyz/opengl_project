#pragma once
#include<string>
#include <memory>
#include "GameObject.h"

#define INPUT_HANDLER_ORDER 1
#define TRANSFORM_ORDER 10
#define RENDER_ORDER 30

class Component {
public: // basic declaration of Component class 
	static const std::size_t Type;
	virtual bool isComponentType(const std::size_t type) const;
	virtual std::size_t getComponentType() const;
	
public: // member
	int order;
	bool is_active;
	GameObject* gameobject; // component is uniquely managered by gameobject, thus it might be safe to use trival pointer here

public: // method
	Component(GameObject* gameobject, int order);
	virtual ~Component() {};
	void uniform_update(float delta);
	bool remove_call();
	bool is_removed();

	template < typename ComponentName >
	std::weak_ptr<ComponentName> getComponent();

protected:
	virtual void start(); // do not initialize here
	virtual void update(float delta);

private:
	bool first_update;
	bool removed;
};

// ------- template method -------//
template < typename ComponentName >
std::weak_ptr<ComponentName> Component::getComponent()
{
	return gameobject->getComponent<ComponentName>();
}

// macro for declaring and defining child classes of component
#define COMPONENT_TO_STRING( x ) #x

#define COMPONENT_DECLARATION( classname )																			\
public:																																								\
	static const std::size_t Type;																													\
	virtual bool isComponentType(const std::size_t type) const;																\
	virtual std::size_t getComponentType() const;																						\

#define COMPONENT_DEFINITION( parent, child )																				\
	const std::size_t child::Type = std::hash<std::string>() ( COMPONENT_TO_STRING(child) );		\
	bool child::isComponentType(const std::size_t type) const																	\
	{																																									\
		if (type == Type)																																		\
			return true;																																			\
		return parent::isComponentType( type);																								\
	}																																									\
	std::size_t child::getComponentType() const																							\
	{																																									\
		return Type;																																			\
	}																																									\