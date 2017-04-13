#include "AttackCollision.h"
#include "fbEngine\BoxCollider.h"

void AttackCollision::Awake()
{
	_BoxColl = AddComponent<BoxCollider>();
}

void AttackCollision::Update()
{

}

void AttackCollision::Create(Vector3& size)
{
	_BoxColl->Create(size);
	transform->SetLocalScale(size);
}