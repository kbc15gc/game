#include"stdafx.h"
#include "AttackCollision.h"

void AttackCollision::Awake()
{
	_BoxColl = AddComponent<BoxCollider>();
}

void AttackCollision::Update()
{
	time += Time::DeltaTime();
	if (time > _lifeTime)
	{
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void AttackCollision::Create(const Vector3& size)
{
	_BoxColl->Create(size);
	transform->SetLocalScale(size);
}

void AttackCollision::Create(const Vector3& size, CollisionMaster master) {
	_BoxColl->Create(size);
	transform->SetLocalScale(size);
	_master = master;
}

void AttackCollision::Create(const Vector3& size, CollisionMaster master, float lifeTime) {
	_BoxColl->Create(size);
	transform->SetLocalScale(size);
	_master = master;
	_lifeTime = lifeTime;
}