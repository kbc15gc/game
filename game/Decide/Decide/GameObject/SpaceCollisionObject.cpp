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
	// �K�v�ɂȂ�����ĂԁB
	//RegistrationObject();


	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), _GetAttachCollision(*_player))) {
		// �v���C���[�ƏՓ˂��Ă���B
		_isHitPlayer = true;
		EnableObjects();	// �����ɏՓ˂��Ă���I�u�W�F�N�g���A�N�e�B�u���B
		_EnableObjectsAdjacent();	// ���͂̋�ԂɏՓ˂��Ă���I�u�W�F�N�g���A�N�e�B�u���B
	}
	else {
		// �Փ˂��Ă��Ȃ��B
		_isHitPlayer = false;
		DisableObjects();
	}
}

void SpaceCollisionObject::AddObjectHitSpace(GameObject& object) {
	if (INSTANCE(PhysicsWorld)->ContactPairTest(GetCollision(), object.GetComponent<Collision>())) {
		// �Փ˂��Ă����̂Œǉ��B
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

	// �ǂꂩ�̃R���W�����R���|�[�l���g�͒ǉ������H�B
	abort();

	return nullptr;
}