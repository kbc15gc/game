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
	// �ŏ��͂��ׂĔ�A�N�e�B�u�ɂ���B
	DisableObjects();
}

void SpaceCollisionObject::UpdateActiveSpace() {
	// �K�v�ɂȂ�����ĂԁB
	//	RegistrationObject();
}

bool SpaceCollisionObject::isHitPlayer() {
	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(*_player))) {
				// �v���C���[�ƏՓ˂��Ă���B
				_isHitPlayer = true;
				return true;
			}
			else {
				// �Փ˂��Ă��Ȃ��B
				_isHitPlayer = false;
				return false;
			}
		}
	}

	abort();
	// �R���W����������B
	return false;
}

SpaceCollisionObject* SpaceCollisionObject::IsHitPlayerAdjacent() {
	for (auto Adjacent : _adjacentSpaceObjects) {
		if (Adjacent->isHitPlayer()) {
			// �Փ˂��Ă����Ԕ����B
			return Adjacent;
		}
	}
	return nullptr;
}

// �Փ˂��Ă���I�u�W�F�N�g�����ׂČ��o�B 
void SpaceCollisionObject::RegistrationObject()
{
	_HitCollisions.clear();
	if (GetCollision()) {
		if (GetCollision()->GetCollisionObj()) {
			vector<Collision*> hitCollisions;
			hitCollisions = INSTANCE(PhysicsWorld)->AllHitsContactTest(GetCollision(), hitCollisions, _attribute);

			for (auto coll : hitCollisions) {
				_HitCollisions.push_back(coll->GetCollisionObj_shared());
			}
		}
	}
}

void SpaceCollisionObject::AddObjectHitSpace(GameObject& object) {
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(object)),_attribute) {
		// �Փ˂��Ă����̂Œǉ��B
		_HitCollisions.push_back(object.GetComponent<Collision>()->GetCollisionObj_shared());
	}
}

void SpaceCollisionObject::_SetActives(bool flg) {
	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		Collision* coll = static_cast<Collision*>((*itr)->getUserPointer());
		if (coll) {
			// �R���W�����폜����ĂȂ��B
			if (coll->gameObject) {
				// �Q�[���I�u�W�F�N�g������B
				coll->gameObject->SetActive(flg);
				if (flg) {
					// �R���W�������A�N�e�B�u���B
					//coll->Activate();
					coll->AddWorld();
				}
				else {
					// �R���W�������X���[�v���B
					//coll->Sleep();
					coll->RemoveWorld();
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
			// ���̋�Ԃ͗אڂ��Ă��Ȃ��B
			DisableObjects();
		}
		for (auto Adjacent : _adjacentSpaceObjects) {
			if (!Adjacent->IsAdjacent(Obj->GetMyNumber())) {
				// ���̋�Ԃ͗אڂ��Ă��Ȃ��B
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

	// �ǂꂩ�̃R���W�����R���|�[�l���g�͒ǉ������H�B
	abort();

	return nullptr;
}