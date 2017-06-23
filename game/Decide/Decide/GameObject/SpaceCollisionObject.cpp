#include "stdafx.h"
#include "GameObject\SpaceCollisionObject.h"
#include "fbEngine\_Object\_Component\_Physics\Collision.h"

void SpaceCollisionObject::Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id,Transform* parent,int attr) {
	if (parent) {
		transform->SetParent(parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rot);
	transform->SetScale(Vector3::one);
	_attribute = attr;
	CollisionObject::Create(id, size,false);
}

void SpaceCollisionObject::Start() {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
	RegistrationObject();
}

void SpaceCollisionObject::UpdateActiveSpace() {
	// 必要になったら呼ぶ。
	//	RegistrationObject();

	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(*_player))) {
				// プレイヤーと衝突している。
				_isHitPlayer = true;
				EnableObjects();	// この空間に含まれているオブジェクトをアクティブ化。
				//DisableObjects();

			}
			else {
				// 衝突していない。
				_isHitPlayer = false;
				DisableObjects();	// この空間に含まれているオブジェクトを非アクティブ化。 
				//EnableObjects();
			}
		}
	}
}

void SpaceCollisionObject::AddObjectHitSpace(GameObject& object) {
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(object)),_attribute) {
		// 衝突していたので追加。
		_HitCollisions.push_back(object.GetComponent<Collision>()->GetCollisionObj_shared());
	}
}

void SpaceCollisionObject::_SetActives(bool flg) {
	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		if ((*itr)->getUserPointer()) {
			// コリジョン削除されてない。
			if (static_cast<Collision*>((*itr)->getUserPointer())->gameObject) {
				// ゲームオブジェクトがある。
				static_cast<Collision*>((*itr)->getUserPointer())->gameObject->SetActive(flg);
				itr++;
				continue;
			}
		}
		itr = _HitCollisions.erase(itr);
	}
}

void SpaceCollisionObject::_EnableObjectsAdjacent() {
	for (auto Adjacent : _adjacentSpaceObjects) {
		Adjacent->EnableObjects();
		//Adjacent->DisableObjects();
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