#include "stdafx.h"
#include "Entity.h"

namespace resource{

	Entity::Entity()
	{
		this->left = NULL;
		this->right = NULL;
	}


	Entity::~Entity()
	{
	}


	Entity::Entity(std::wstring name)
	{
		this->left = NULL;
		this->right = NULL;
		this->name = name;
	}

	int Entity::compare(const Entity& node)
	{
		int n = this->name.compare(node.name);
		return n;
	}

	Entity* Entity::getLeft()
	{
		return this->left;
	}

	Entity* Entity::getRight()
	{
		return this->right;
	}

	void Entity::addLeft(Entity* n)
	{
		this->left = n;
	}

	void Entity::addRight(Entity* n)
	{
		this->right = n;
	}

	std::wstring Entity::getName()
	{
		return this->name;
	}

}