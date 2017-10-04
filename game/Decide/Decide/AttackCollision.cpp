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
	_time += Time::DeltaTime();
	if (_isCreateCollision) {
		// �R���W��������������Ă���B
		if (_lifeTime >= 0.0f && _time > _lifeTime)
		{
			// �����������łȂ����������߂����B

			// �폜�B
			INSTANCE(GameObjectManager)->AddRemoveList(this);
		}
		else {
			// �������܂�����B

			// �Փ˔���B
			DetectionCollision();
		}
	}
	else {
		// �R���W��������������Ă��Ȃ��B
		CreateCollision();
	}
}

void AttackCollision::LateUpdate()
{
}

void AttackCollision::DetectionCollision() {
	// �Փ˂��Ă���R���W���������ׂĎ擾����B
	btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();

	for (auto itr = _HitCollisions.begin(); itr != _HitCollisions.end();) {
		bool isHit = false;	// �o�^���Ă���R���W���������݂��Փ˂��Ă��邩�B
		for (int idx = 0; idx < collisions.size(); idx++) {
			if ((*itr).get() == collisions[idx]) {
				// ���ɓo�^����Ă���R���W�������Փ˂��Ă���B
				isHit = true;

				// �Փ˂��Ă���ԌĂё�����R�[���o�b�N�����B
				_CallBackStay((*itr).get());
				// �R�[���o�b�N���Ă񂾂��͔̂z�񂩂珜���B
				collisions.remove(collisions[idx]);
				break;
			}
			else if (collisions[idx]->getUserIndex() != Collision_ID::PLAYER && 
				collisions[idx]->getUserIndex() != Collision_ID::ENEMY) {
				// �R�[���o�b�N���Ă΂Ȃ��R���W�����������B
				collisions.remove(collisions[idx]);
			}
		}

		if (!isHit) {
			// �o�^����Ă����R���W�������Փ˂��Ă��Ȃ������B
			// �Փ˂��O�ꂽ���ɌĂяo���R�[���o�b�N�����B
			_CallBackExit((*itr).get());
			//(*itr).reset();	// �V�F�A�[�h�|�C���^�̎Q�ƃJ�E���^�𖾎��I�ɉ�����(�K�v�H)�B
			itr = _HitCollisions.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (int idx = 0; idx < collisions.size();idx++) {
		// �Փ˂������u�ԂɌĂяo���R�[���o�b�N�����B
		_CallBackEnter(collisions[idx]);
		// �Փ˃��X�g�ɒǉ��B
		_HitCollisions.push_back(static_cast<Collision*>(collisions[idx]->getUserPointer())->GetCollisionObj_shared());
	}
}

void AttackCollision::_CallBackEnter(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// �G�ƏՓ˂����B
				// �Q�[���I�u�W�F�N�g������B
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionEnter(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// �v���C���[�ƏՓ˂����B
				static_cast<Player*>(obj)->HitAttackCollisionEnter(this);
			}
			break;
		case CollisionMaster::Other:
			// ���̑������������U���B
			// ���b��B�Ƃ肠�����������Ȃ��B
			break;
		}
	}
}

// �Փ˂��Ă���ԌĂё�����R�[���o�b�N�����B
void AttackCollision::_CallBackStay(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// �G�ƏՓ˂����B
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// �v���C���[�ƏՓ˂����B
				static_cast<Player*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Other:
			// ���̑������������U���B
			// ���b��B�Ƃ肠�����������Ȃ��B
			break;
		}
	}
}

void AttackCollision::_CallBackExit(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (coll->getUserIndex() == Collision_ID::ENEMY) {
				// �G�ƏՓ˂����B
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (coll->getUserIndex() == Collision_ID::PLAYER) {
				// �v���C���[�ƏՓ˂����B
				static_cast<Player*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Other:
			// ���̑������������U���B
			// ���b��B�Ƃ肠�����������Ȃ��B
			break;
		}
	}
}

void AttackCollision::Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, float waitTime, Transform* Parent) {

	_DamageInfo = move(info);

	_lifeTime = lifeTime;	// ������ۑ��B
	_master = master;		// �R���W�����̐����҂�ۑ��B
	_waitTime = waitTime;	// �����҂����ԕۑ��B

	static_cast<BoxCollider*>(_Colider)->Create(size);		// �R���C�_�[����(���Ƃ肠�����b��I�Ƀ{�b�N�X�Œ�)�B

	// �R���W�����ɐݒ肷��Transform���ݒ�B
	if (Parent) {
		transform->SetParent(Parent);
	}
	transform->SetLocalPosition(pos);
	transform->SetLocalRotation(rotation);

	// �R���W���������B
	CreateCollision();
}

void AttackCollision::CreateCollision() {
	if (_time >= _waitTime) {
		// �҂����Ԃ��J�E���g���I�����B
		transform->UpdateTransform();
		_Gost->Create(_Colider, Collision_ID::ATTACK);	// �S�[�X�g�R���W���������B
		_isCreateCollision = true;
		_time = 0.0f;
	}
}
