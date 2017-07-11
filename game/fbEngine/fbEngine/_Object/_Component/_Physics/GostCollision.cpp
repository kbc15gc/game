#include"fbstdafx.h"
#include "GostCollision.h"

GostCollision::~GostCollision()
{
}

void GostCollision::Awake()
{
	Collision::Awake();
}

void GostCollision::Create(Collider * shape, int id, bool isAddWorld)
{
	//コリジョン生成
	Collision::Create(new btGhostObject, shape, id,Vector3::zero,isAddWorld);
	//生ポインタ取得してキャスト
	_GostObject = btGhostObject::upcast(this->_CollisionObject.get());
	//今設定されているフラグを取得
	int flags = _GostObject->getCollisionFlags();
	//他のコリジョンと衝突しないようにフラグを設定。
	_GostObject->setCollisionFlags(flags | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

	_MyObjectType = CollisionObjectType::Ghost;
	_physicsType = PhysicsType::Kinematick;	// ゴーストはとりあえずキネマティック剛体。
}

void GostCollision::Update()
{
	Collision::Update();
}

const btAlignedObjectArray<btCollisionObject*>& GostCollision::GetPairCollisions()
{
	//自身と重なっているコリジョン取得
	return _GostObject->getOverlappingPairs();
}

void GostCollision::_AddWorldSubClass() {
	INSTANCE(PhysicsWorld)->AddCollision(this);
}

void GostCollision::_RemoveWorldSubClass() {
	INSTANCE(PhysicsWorld)->RemoveCollision(this);
}
