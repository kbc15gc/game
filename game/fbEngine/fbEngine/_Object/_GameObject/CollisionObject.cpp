#include"fbstdafx.h"
#include "CollisionObject.h"

void CollisionObject::Create(int ID, Vector3 size)
{
	transform->SetLocalScale(Vector3(size.x, size.y, size.z));
	_Shape->Create(Vector3(size.x, size.y, size.z));
	_Coll->Create(_Shape,ID);
}

void CollisionObject::Awake()
{
	_Shape = AddComponent<BoxCollider>();
	_Coll = AddComponent<GostCollision>();
}

void CollisionObject::Update()
{
	
}