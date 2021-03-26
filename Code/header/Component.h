#pragma once
#include<string>
#include <memory>

class Component {
public: // type
	static const std::size_t Type;
	virtual bool isComponentType(const std::size_t type) const;

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