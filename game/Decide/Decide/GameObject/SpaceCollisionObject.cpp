#include "stdafx.h"
#include "GameObject\SpaceCollisionObject.h"
#include "fbEngine\_Object\_Component\_Physics\Collision.h"

void SpaceCollisionObject::Create(const Vector3& pos, const Quaternion& rot, const Vector3& scale, int id) {
	transform->SetPosition(pos);
	transform->SetRotation(rot);
	CollisionObject::Create(id, scale);
}

void SpaceCollisionObject::Start() {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
	RegistrationObject();
}

void SpaceCollisionObject::Update() {
	// 必要になったら呼ぶ。
	//RegistrationObject();


	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(*_player))) {
		// プレイヤーと衝突している。
		_isHitPlayer = true;
		EnableObjects();	// 自分に衝突しているオブジェクトをアクティブ化。
		_EnableObjectsAdjacent();	// 周囲の空間に衝突しているオブジェクトをアクティブ化。
	}
	else {
		// 衝突していない。
		_isHitPlayer = false;
		DisableObjects();
	}
}

void SpaceCollisionObject::AddObjectHitSpace(GameObject& object) {
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), object.GetComponent<Collision>())) {
		// 衝突していたので追加。
		_HitCollisions.push_back(object.GetComponent<Collision>()->GetCollisonObj());
	}
}

void SpaceCollisionObject::_SetActives(bool flg) {
	for (int idx = 0; idx < _HitCollisions.size(); idx++) {
		static_cast<Collision*>(_HitCollisions[idx]->getUserPointer())->gameObject->SetActive(flg);
	}
}

void SpaceCollisionObject::_EnableObjectsAdjacent() {
	for (auto Adjacent : _adjacentSpaceObjects) {
		Adjacent->EnableObjects();
	}
}

Collision* SpaceCollisionObject::_GetAttachCollision(GameObject& object) {
	Collision* coll = nullptr;
	coll = object.GetComponent<Collision>();
	if (coll) {
		return coll;
	}
	coll = object.GetComponent<GostCollision>();
	if (coll) {
		return coll;
	}
	coll = object.GetComponent<RigidBody>();
	if (coll) {
		return coll;
	}

	// どれかのコリジョンコンポーネントは追加した？。
	abort();

	return nullptr;
}