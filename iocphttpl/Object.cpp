#include "stdafx.h"
#include "Object.h"

namespace resource{

	int Object::ctr = 0;

	Object::Object(void)
	{
		if (Object::ctr == NULL)
		{
			Object::ctr = 1;
		}
		else if (Object::ctr == 0)
		{
			Object::ctr++;
		}
		this->Id = Object::ctr;
	}


	Object::~Object(void)
	{
	}


	int Object::getId()
	{
		return this->Id;
	}

}