#include"fbstdafx.h"
#include "GameObject.h"
#include "_Object\_Component\_Physics\Collision.h"

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

Collision* GameObject::GetAttachCollision() {
	Collision* coll = nullptr;
	coll = GetComponent<Collision>();
	if (coll) {
		return coll;
	}
	coll = GetComponent<GostCollision>();
	if (coll) {
		return coll;
	}
	coll = GetComponent<RigidBody>();
	if (coll) {
		return coll;
	}

	return nullptr;
}