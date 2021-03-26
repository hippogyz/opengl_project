#pragma once
#include<string>
#include <memory>

class Component {
public: // basic declaration of Component class 
	static const std::size_t Type;
	virtual bool isComponentType(const std::size_t type) const;
	virtual std::size_t getComponentType() const;

public: // member
	int order;
	bool is_active;

public: // method
	Component(bool is_active, int order);
	void uniform_update(float delta);
	bool remove_call();

protected:
	virtual void start();
	virtual void update(float delta);

private:
	bool first_update;
	bool removed;
};


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