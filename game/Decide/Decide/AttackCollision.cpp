#include "AttackCollision.h"
#include "fbEngine\BoxCollider.h"

void AttackCollision::Awake()
{
	_BoxColl = AddComponent<BoxCollider>();
}

void AttackCollision::Update()
{
	time += Time::DeltaTime();
	if (time > 3)
	{
		GameObjectManager::AddRemoveList(this);
	}
}

void AttackCollision::Create(Vector3& size)
{
	_BoxColl->Create(size);
	transform->SetLocalScale(size);
}