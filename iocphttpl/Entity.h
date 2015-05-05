#pragma once
#include "Object.h"


using namespace std;

namespace resource{

	class Entity : public Object
	{
	public:
		Entity();
		~Entity();

	public:
		Entity(std::wstring name);
		int compare(const Entity& node);
		Entity* getLeft();
		Entity* getRight();
		void addLeft(Entity* n);
		void addRight(Entity* n);
		std::wstring getName();
	private:
		std::wstring name;
		Entity* left;
		Entity* right;
		//IasProperty<int> property;
	};

}