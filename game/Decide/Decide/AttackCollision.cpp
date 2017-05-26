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
			// �����������łȂ��B

			// �폜�B
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
	else {
		// �������܂�����B

		// �Փ˔���B
		// ���Փ˂��Ă���R���W���������ׂĎ擾����B
		btAlignedObjectArray<btCollisionObject*> collisions = _Gost->GetPairCollisions();
		for (int idx = 0; idx < collisions.size();idx++) {
			switch (_master) {
			case CollisionMaster::Player:
				// �v���C���[�����������U���B
				if (collisions[idx]->getUserIndex() == Collision_ID::ENEMY) {
					// �G�ƏՓ˂����B
					static_cast<Player*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
				}
				break;
			case CollisionMaster::Enemy:
				// �G���G�����������U���B
				if (collisions[idx]->getUserIndex() == Collision_ID::PLAYER) {
					// �v���C���[�ƏՓ˂����B
					static_cast<EnemyCharacter*>(static_cast<Collision*>(collisions[idx]->getUserPointer())->gameObject)->HitAttackCollision(this);
				}
				break;
			case CollisionMaster::Other:
				// ���̑������������U���B
				// ���b��B�Ƃ肠�����������Ȃ��B
				break;
			}
		}
	}
}

