#pragma once
#include "_Object\_Component\Component.h"

//2Dのコリジョン
class Collison2D:public Component
{
public:
	Collison2D(GameObject* g, Transform* t) : Component(g, t, typeid(this).name())
	{
		
	}
	Collison2D(GameObject* g, Transform* t,const char* classname) : Component(g, t,classname)
	{

	}
	virtual ~Collison2D() {};
};