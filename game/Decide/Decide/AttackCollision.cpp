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
	if (_lifeTime >= 0.0f && _time > _lifeTime)
	{
		// �����������łȂ����������߂����B

		if (_isLifeOverDelete) {
			// �폜�B
			INSTANCE(GameObjectManager)->AddRemoveList(this);
			_isAlive = false;
		}
		else {
			this->SetActive(false);
			_isAlive = false;
		}
	}
	else {
		// �������܂�����B

		// �Փ˔���B
		DetectionCollision();
	}
}

void AttackCollision::LateUpdate()
{
}

void AttackCollision::DetectionCollision() {
	// �Փ˂��Ă���R���W���������ׂĎ擾����B
	vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> hit;
	int attr = Collision_ID::ENEMY | Collision_ID::BOSS | Collision_ID::PLAYER | Collision_ID::GROUND | Collision_ID::BUILDING | Collision_ID::DRARIAN;
	fbPhysicsCallback::AllHitsContactResultCallback callback;
	INSTANCE(PhysicsWorld)->AllHitsContactTest(_Gost, &hit, &callback, attr);

	//btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();
	int max = static_cast<int>(_hitInfos.size());
	for (int num = 0; num < max; ) {
		// �o�^����Ă���I�u�W�F�N�g�������B

		bool isHit = false;	// �o�^���Ă���I�u�W�F�N�g�����݂��Փ˂��Ă��邩�B

		if (_hitInfos[num]->coll->getUserPointer() == nullptr)
		{
			// �o�^����Ă����I�u�W�F�N�g���폜���ꂽ�B

			_hitInfos[num]->object = nullptr;
		}
		else {

			// �t���O�������B
			_hitInfos[num]->isCallStay = false;

			int max2 = static_cast<int>(hit.size());
			for (int idx = 0; idx < max2; idx++) {
				// �V�����擾�����Փ˃R���W�����������B

				GameObject* obj = _CollisionObjectToGameObject(hit[idx]->collision->GetCollisionObj());
				if (obj && _hitInfos[num]->object == obj) {
					// �o�^����Ă���I�u�W�F�N�g���Փ˂��Ă����B

					if (!_hitInfos[num]->isCallStay) {
						// �X�e�C�R�[���o�b�N���܂��Ă΂�Ă��Ȃ��B

						// �Փ˂��Ă���ԌĂё�����R�[���o�b�N�����B
						_CallBackStay(_hitInfos[num]->object);
						_hitInfos[num]->isCallStay = true;
					}

					// �o�^����Ă�����̂͐V�����擾�����z�񂩂珜���B
					hit.erase(hit.begin() + idx);
					max2--;
					isHit = true;
				}
			}
		}

		if (!isHit) {
			// �o�^����Ă����I�u�W�F�N�g���Փ˂��Ă��Ȃ������B

			if (_hitInfos[num]->object) {
				// �Փ˂��O�ꂽ���ɌĂяo���R�[���o�b�N�����B
				_CallBackExit(_hitInfos[num]->object);
			}

			_hitInfos.erase(_hitInfos.begin() + num);

			// �v�f�������炷�B
			max--;
		}
		else {
			// ���̗v�f�ɐi�ށB
			num++;
		}
	}


	vector<GameObject*> add;
	for (int idx = 0; idx < static_cast<int>(hit.size()); idx++) {
		// �V�����Փ˂����I�u�W�F�N�g�B

		if (!_isHit) {
			_isHit = true;
		}

		GameObject* obj = _CollisionObjectToGameObject(hit[idx]->collision->GetCollisionObj());

		bool flg = true;
		for (auto& a : add) {

			if (a == obj) {
				// �ǉ��ς݃I�u�W�F�N�g�Ɠ������͖̂����B
				flg = false;
				break;
			}
		}

		if (flg) {
			if (hit[idx]->collision->GetID()) {
				// �Փ˂����u�ԂɌĂяo���R�[���o�b�N�����B
				_CallBackEnter(hit[idx]->collision->GetCollisionObj());

				// �Փ˃��X�g�ɒǉ��B
				unique_ptr<HitObjectInfo> info(new HitObjectInfo);

				info->coll = hit[idx]->collision->GetCollisionObj_shared();

				info->object = obj;
				_hitInfos.push_back(move(info));
				add.push_back(obj);
			}

		}
	}

	hit.clear();
	add.clear();

}

void AttackCollision::_CallBackEnter(btCollisionObject* coll) {
	GameObject* obj = _CollisionObjectToGameObject(coll);
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (coll->getUserIndex() == Collision_ID::ENEMY || coll->getUserIndex() == Collision_ID::BOSS || coll->getUserIndex() == Collision_ID::DRARIAN) {
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
void AttackCollision::_CallBackStay(GameObject* obj) {
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (obj->GetAttachCollision()->GetID() == Collision_ID::ENEMY || obj->GetAttachCollision()->GetID() == Collision_ID::BOSS) {
				// �G�ƏՓ˂����B
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionStay(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (obj->GetAttachCollision()->GetID() == Collision_ID::PLAYER) {
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

void AttackCollision::_CallBackExit(GameObject* obj) {
	if (obj) {
		switch (_master) {
		case CollisionMaster::Player:
			// �v���C���[�����������U���B
			if (obj->GetAttachCollision()->GetID() == Collision_ID::ENEMY || obj->GetAttachCollision()->GetID() == Collision_ID::BOSS) {
				// �G�ƏՓ˂����B
				static_cast<EnemyCharacter*>(obj)->HitAttackCollisionExit(this);
			}
			break;
		case CollisionMaster::Enemy:
			// �G���G�����������U���B
			if (obj->GetAttachCollision()->GetID() == Collision_ID::PLAYER) {
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

void AttackCollision::Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, ReactionType reactionType, Transform* Parent, bool isLifeOverDelete) {

	_DamageInfo = move(info);

	_lifeTime = lifeTime;	// ������ۑ��B
	_master = master;		// �R���W�����̐����҂�ۑ��B
	_reactionType = reactionType;
	_isLifeOverDelete = isLifeOverDelete;
	_Colider->Create(size);		// �R���C�_�[����(���Ƃ肠�����b��I�Ƀ{�b�N�X�Œ�)�B

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
	_isAlive = true;
	transform->UpdateTransform();
	_Gost->Create(_Colider, Collision_ID::ATTACK);	// �S�[�X�g�R���W���������B
	_time = 0.0f;
}
