/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/
#include"fbstdafx.h"
#include "CharacterController.h"
#include "_Object\_Component\_Physics\Collider.h"

#define TEST_CHARACON_XZ
#define TEST_CHARCON_Y


void CCharacterController::Init(GameObject* Object, Transform* tramsform, Vector3 off, int type, Collider* capsule, float gravity, int attributeXZ, int attributeY, bool isAddWorld)
{
	//�R���W�����쐬�B
	//�d�͐ݒ�B
	SetGravity(gravity);

	// �Փ˂����o������������ݒ�B
	m_attributeXZ = attributeXZ;
	m_attributeY = attributeY;

	m_rigidBody = gameObject->AddComponent<RigidBody>();
	//���W�b�h�{�f�B�쐬
	m_rigidBody->Create(0.0f, capsule, type, Vector3::zero, off,Collision::PhysicsType::Kinematick,isAddWorld);
	//�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj())->setSleepingThresholds(0, 0);


	m_collider = capsule;

	btVector3 localScaling(1.0f,1.0f,1.0f); // Transform�̃X�P�[���l�B
	localScaling = m_collider->GetBody()->getLocalScaling();
	_halfSize = m_rigidBody->GetShape()->GetHalfSize();
	_halfSize.x *= localScaling.getX();
	_halfSize.y *= localScaling.getY();
	_halfSize.z *= localScaling.getZ();

	//���Ƃ��Ƃ̃t���O���c�����܂ܐV�����t���O��ǉ��B
	m_rigidBody->GetCollisionObj()->setCollisionFlags(m_rigidBody->GetCollisionObj()->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
}
void CCharacterController::Execute()
{
	//���x�ɏd�͉����x��������B
	m_moveSpeed.y += m_gravity;
	Vector3 nowPosTmp; 
	Vector3 nextPosTmp;
	Vector3 originalXZDir;
	//���̈ړ���ƂȂ���W���v�Z����B
	{
		nextPosTmp = nowPosTmp = m_rigidBody->GetOffsetPos();	// �ړ��O�̃R���W�����̍��W(���_)��ݒ�B
		nextPosTmp.y = nowPosTmp.y = nowPosTmp.y - m_rigidBody->GetShape()->GetHalfSize().y;	// �ʒu�����R���W�����̑����ɍ��킹��B
		//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
		Vector3 addPos = m_moveSpeed;
		addPos.Scale(Time::DeltaTime());
		nextPosTmp += addPos;

		originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
	}


#ifdef TEST_CHARACON_XZ

	//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
	{
		int loopCount = 0;
		while (true) {
			Vector3 NowToNext(nextPosTmp - nowPosTmp);
			NowToNext.y = 0.0f;
			if (NowToNext.Length() <= FLT_EPSILON) {
				//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
				//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
				//�ƂĂ��������l�̂��Ƃł��B
				break;
			}
			//���C���쐬�B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̑����̒n�_�B
			// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
			start.setOrigin(btVector3(nowPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nowPosTmp.z));
			// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
			end.setOrigin(btVector3(nextPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nextPosTmp.z));

			fbPhysicsCallback::SweepResultWall callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeXZ;
			//�Փˌ��o�B
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

			if (callback.isHit) {
				//���������B
				//�ǁB
				Vector3 vT0, vT1;
				//XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ肷��B
				vT0.Set(nextPosTmp.x, 0.0f, nextPosTmp.z);
				vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
				//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
				Vector3 vMerikomi;
				vMerikomi = vT0 - vT1;
				//XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
				Vector3 hitNormalXZ = callback.hitNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
				float fT0 = hitNormalXZ.Dot(vMerikomi);
				//�����߂��Ԃ��x�N�g�������߂�B
				//�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
				Vector3 vOffset;
				vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
				nextPosTmp += vOffset;
				Vector3 currentDir;
				currentDir = nextPosTmp - nowPosTmp;
				currentDir.y = 0.0f;
				currentDir.Normalize();
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//�p�ɓ��������̃L�����N�^�̐U����h�~���邽�߂ɁA
					//�ړ��悪�t�����ɂȂ�����ړ����L�����Z������B
					nextPosTmp.x = nowPosTmp.x;
					nextPosTmp.z = nowPosTmp.z;
					break;
				}
			}
			else {
				//�ǂ��Ƃ�������Ȃ��̂ŏI���B
				break;
			}
			loopCount++;
			if (loopCount >= 5) {
				break;
			}
		}
	}
#endif

#ifdef TEST_CHARCON_Y

	//XZ�̈ړ��͊m��B
	nowPosTmp.x = nextPosTmp.x;
	nowPosTmp.z = nextPosTmp.z;
	//�������𒲂ׂ�B
	{

		Vector3 addPos = nextPosTmp - nowPosTmp;

		//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�͉��m�肵���J�v�Z���R���C�_�[�̒��S�ʒu(Y�����͈ړ��O)�B
		start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y, nextPosTmp.z));
		//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
		//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
		//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
		// �����̏����͍⓹�̏����ŕK�v�B
		Vector3 endPos;
		endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//�W�����v���Ƃ��ŏ㏸���B
				//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������(����Ȃ�)�̂ŁA�ړ���̍��W���牺�𒲂ׂ�B
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
				endPos.y += addPos.y;
			}
		}
		else {
			//�n�ʏ�ɂ���ꍇ��1m��������B
			// �������~���Ƃ��ɃK�^�K�^���Ȃ��悤�B
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		//�X�^�[�g�ƃG���h�̍�
		btVector3 Sub = start.getOrigin() - end.getOrigin();
		//����0.0001�ȏ�Ȃ�Փˌ��o����B
		if (fabs(Sub.length()) > 0.0001f)
		{
			//�Փˌ��o�B
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;

			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//�n�ʂɐڐG���Ă���B

				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosTmp.y = callback.hitPos.y;
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B

				m_isOnGround = false;
			}
		}
	}
#endif

	nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// �����̈ʒu�ɐݒ肵�Ă����̂ŃR���W�����̒��S�ɖ߂��B
	//�ړ��m��B
	transform->SetPosition(nextPosTmp - m_rigidBody->GetOffset());	// nextPosTmp�̓��f���̌��_�ƃC�R�[���łȂ��\��������̂�Offset�����Z���Đݒ肷��B
	m_rigidBody->Update();	// transform���X�V�����̂ŃR���W������Transfiorm���X�V����B
	//���̂𓮂����B
	m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);
}
