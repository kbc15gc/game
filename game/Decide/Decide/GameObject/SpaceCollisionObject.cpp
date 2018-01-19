#include "stdafx.h"
#include "GameObject\SpaceCollisionObject.h"
#include "fbEngine\_Object\_Component\_Physics\Collision.h"

void SpaceCollisionObject::Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id,Transform* parent,int attr,const Vector3& myNumber) {
	if (parent) {
		transform->SetParent(parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rot);
	transform->SetScale(Vector3::one);
	_attribute = attr;
	CollisionObject::Create(id, size,false);

	_myNumber = myNumber;
}

void SpaceCollisionObject::Start() {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
	RegistrationObject();
	// 最初はすべて非アクティブにする。
	DisableObjects();
}

void SpaceCollisionObject::UpdateActiveSpace() {
	// 必要になったら呼ぶ。
	//	RegistrationObject();
}

bool SpaceCollisionObject::isHitPlayer() {
	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _player->GetAttachCollision())) {
				// プレイヤーと衝突している。
				_isHitPlayer = true;
				return true;
			}
			else {
				// 衝突していない。
				_isHitPlayer = false;
				return false;
			}
		}
	}

	abort();
	// コリジョン無いよ。
	return false;
}

SpaceCollisionObject* SpaceCollisionObject::IsHitPlayerAdjacent() {
	for (auto Adjacent : _adjacentSpaceObjects) {
		if (Adjacent->isHitPlayer()) {
			// 衝突している空間発見。
			return Adjacent;
		}
	}
	return nullptr;
}

// 衝突しているオブジェクトをすべて検出。 
void SpaceCollisionObject::RegistrationObject()
{
	_HitCollisions.clear();
	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> hitCollisions;
			fbPhysicsCallback::AllHitsContactResultCallback callback;
			INSTANCE(PhysicsWorld)->AllHitsContactTest(GetCollision(), &hitCollisions, &callback, _attribute);

			for (auto& info : hitCollisions) {
				_HitCollisions.push_back(info->collision->GetCollisionObj_shared());
			}
		}
	}
}

void SpaceCollisionObject::AddObjectHitSpace(GameObject& object) {
	Collision* coll = object.GetAttachCollision();
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), coll,_attribute)) {
		// 衝突していたので追加。
		_HitCollisions.push_back(coll->GetCollisionObj_shared());
	}
}

void SpaceCollisionObject::_SetActives(bool flg) {
	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		Collision* coll = static_cast<Collision*>((*itr)->getUserPointer());
		if (coll) {
			// コリジョン削除されてない。

			if (coll->gameObject) {
				// ゲームオブジェクトがある。
				coll->gameObject->SetActive(flg);
				if (flg) {
					// コリジョンをアクティブ化。

					//coll->Activate();

					//coll->AddWorld();

					if (coll->GetCollisionType == Collision::CollisionObjectType::Rigid) {
						// 剛体アクティブ。
						static_cast<btRigidBody*>(coll->GetCollisionObj())->wantsSleeping();
					}
				}
				else {
					// コリジョンをスリープ化。

					//coll->Sleep();

					//coll->RemoveWorld();

					if (coll->GetCollisionType == Collision::CollisionObjectType::Rigid) {
						// 剛体スリープ。
						static_cast<btRigidBody*>(coll->GetCollisionObj())->wantsSleeping();
					}
				}
				itr++;
				continue;
			}
		}
		itr = _HitCollisions.erase(itr);
	}
	_isActive = flg;
}

void SpaceCollisionObject::EnableObjectsAdjacent() {
	for (auto Adjacent : _adjacentSpaceObjects) {
		Adjacent->EnableObjects();
	}
}

void SpaceCollisionObject::DisableNotAdjacent(const SpaceCollisionObject* Obj) {
	if (Obj) {
		if (!IsAdjacent(Obj->GetMyNumber())) {
			// この空間は隣接していない。
			DisableObjects();
		}
		for (auto Adjacent : _adjacentSpaceObjects) {
			if (!Adjacent->IsAdjacent(Obj->GetMyNumber())) {
				// この空間は隣接していない。
				Adjacent->DisableObjects();
			}
		}
	}
	else {
		DisableObjects();
		for (auto Adjacent : _adjacentSpaceObjects) {
			Adjacent->DisableObjects();
		}
	}
}

