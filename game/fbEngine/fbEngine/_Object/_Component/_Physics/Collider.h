#pragma once
#include "_Object\_Component\Component.h"

//�����蔻��̌`��
class Collider :public Component
{
public:
	Collider(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		
	};
	Collider(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{

	};
	~Collider(){}
	virtual btCollisionShape* GetBody() = 0 ;
};