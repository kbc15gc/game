#include"stdafx.h"
#include "AttackCollision.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Enemy\EnemyCharacter.h"

void AttackCollision::Awake()
{
	// ���Ƃ肠�����b��B
	// ���`��̃R���C�_�[�𐶐��B
	_Colider = AddComponent<BoxCollider>();
	_Gost = AddComponent<GostCollision>();
}

void AttackCollision::Update()
{
	time += Time::DeltaTime();
	if (_lifeTime >= 0.0f && time > _lifeTime)
	{
		// �����������łȂ����������߂����B
		// �������������łȂ��B

			// �폜�B
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		// �������܂�����B

		// �Փ˔���B
		DetectionCollision();
	}
}

void AttackCollision::LateUpdate()
{
	btAlignedObjectArray<btCollisionObject*> colls = _Gost->GetPairCollisions();

	FOR(i, colls.size())
		((Collision*)(colls.at(i)->getUserPointer()))->gameObject;
}

void AttackCollision::DetectionCollision() {
	// �Փ˂��Ă���R���W���������ׂĎ擾����B
	//vector<Collision*> collisions = INSTANCE(PhysicsWorld)->AllHitsContactTest(this->_Gost);

	btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();

	for (int idx = 0; idx < collisions.size(); idx++) {
		// �擾�����R���W�����̏����Q�Ƃ��đΉ�����R�[���o�b�N���Ăяo���B

		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (collisions[idx]->getUserIndex() == Collision_ID::ENEMY) {
				// �G�ƏՓ˂����B
				static_cast<EnemyCharacter*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (collisions[idx]->getUserIndex() == Collision_ID::PLAYER) {
				// �v���C���[�ƏՓ˂����B
				static_cast<Player*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
			}
			break;
		case CollisionMaster::Other:
			// ���̑������������U���B
			// ���b��B�Ƃ肠�����������Ȃ��B
			break;
		}
	}

	
}

GostCollision* AttackCollision::Create(int attack, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, Transform* Parent) {
	_lifeTime = lifeTime;	// ������ۑ��B
	_master = master;		// �R���W�����̐����҂�ۑ��B
	_AttackDamage = attack;		// �U���͂�ۑ��B
	static_cast<BoxCollider*>(_Colider)->Create(size);		// �R���C�_�[����(���Ƃ肠�����b��I�Ƀ{�b�N�X�Œ�)�B
	if (Parent) {
		transform->SetParent(Parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rotation);

	transform->UpdateTransform();
	_Gost->Create(_Colider, Collision_ID::ATTACK);	// �S�[�X�g�R���W���������B
	return _Gost;
}