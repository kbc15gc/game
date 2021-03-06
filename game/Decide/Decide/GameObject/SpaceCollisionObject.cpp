#include "stdafx.h"
#include "GameObject\SpaceCollisionObject.h"
#include "fbEngine\_Object\_Component\_Physics\Collision.h"
#include "SplitSpace.h"
#include "Village\EventManager.h"

void SpaceCollisionObject::Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id,Transform* parent,int attr,const Int3& myNumber) {
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

	//使うオブジェクトを切り替える。
	auto Target = (INSTANCE(EventManager)->GetEventID() == Event::EventID::EventCameraF) ? GetEventCamera() : _player;
	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), Target->GetAttachCollision())) {
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
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), coll, _attribute)) {
		// 衝突していたので追加。
		_HitCollisions.push_back(coll->GetCollisionObj_shared());
		object.SetActive(this->_isActive);
	}
}

void SpaceCollisionObject::_SetActives(bool flg) {
	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		Collision* coll = static_cast<Collision*>((*itr)->getUserPointer());
		if (coll) {
			// コリジョン削除されてない。

			if (coll->gameObject) {
				// ゲームオブジェクトがある。

				if (strcmp(coll->gameObject->GetName(), "MaouSiro.X") == 0) {
					// 魔王城は無視。

					itr = _HitCollisions.erase(itr);
					continue;
				}

				coll->gameObject->SetActive(flg);
				if (flg) {
					// コリジョンをアクティブ化。

					//coll->Activate();

					//coll->AddWorld();

					if (coll->GetCollisionType() == Collision::CollisionObjectType::Rigid) {
						// 剛体アクティブ。
						static_cast<btRigidBody*>(coll->GetCollisionObj())->wantsSleeping();
					}
				}
				else {
					// コリジョンをスリープ化。

					//coll->Sleep();

					//coll->RemoveWorld();

					if (coll->GetCollisionType() == Collision::CollisionObjectType::Rigid) {
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
		Int3 move = Obj->GetMyNumber() - GetMyNumber();	// この空間から渡された空間までの移動量を算出。
		if (!IsAdjacent(Obj->GetMyNumber())) {
			// この空間は隣接していない。
			DisableObjects();
		}
		for (auto Adjacent : _adjacentSpaceObjects) {
			// この空間と隣接する空間から新しい空間とも隣接していない空間を探索。
			if (!Adjacent->IsAdjacent(Obj->GetMyNumber())) {
				// この空間は隣接していない。
				Adjacent->DisableObjects();

				// 二つの空間にまたがるオブジェクトを非アクティブにした場合を考えて、アクティブ化した空間をもう一度探索できるように設定。
				Int3 work = Adjacent->GetMyNumber() + move;
				if (_splitSpace) {
					SpaceCollisionObject* space = _splitSpace->GetSpaceObject(work);
					if (space) {
						space->SetIsActive(false);
					}
				}
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

