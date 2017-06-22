#include"fbstdafx.h"
#include "CollisionObject.h"

void CollisionObject::Create(int ID, Vector3 size,bool isAddWorld)
{
	_Shape->Create(size);
	_Coll->Create(_Shape,ID,isAddWorld);
}

void CollisionObject::Awake()
{
	_Shape = AddComponent<BoxCollider>();
	_Coll = AddComponent<GostCollision>();
}

void CollisionObject::Update()
{
	
}