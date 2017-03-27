#include "GameObject.h"

GameObject::GameObject():
	Object(),
	_Active(true),
	_Discard(true)
{
	transform = AddComponent<Transform>();
}

GameObject::GameObject(const char* name):
	Object(name),
	_Active(true),
	_Discard(true)
{
	transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
	_Components.Release();
}

//void GameObject::Active(bool act)
//{
//	Object::Active(act);
//	//�q������
//	if (transform->ChildCnt() > 0)
//	{
//		//����������o��
//		map<char*, transform*> child = transform->Children();
//		//�C�e���[�^�擾
//		map<char*, transform*>::iterator it = child.begin();
//		//�I���܂Ń��[�v
//		while (it != child.end())
//		{
//			//�q�𓯂���Ԃɂ���
//			it->second->gameObject->_Active = act;
//			it++;
//		}
//	}
//}