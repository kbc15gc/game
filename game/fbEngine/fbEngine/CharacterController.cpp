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

	btVector3 size(0.0f,0.0f,0.0f);	// �R���C�_�[�T�C�Y�B
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
	m_moveSpeed.y += m_gravity * Time::DeltaTime();
	//���̈ړ���ƂȂ���W���v�Z����B
	Vector3 nextPosition = transform->GetPosition();
	//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
	Vector3 addPos = m_moveSpeed;
	addPos.Scale(Time::DeltaTime());
	nextPosition.Add(addPos);

	//���݂̍��W���玟�̈ړ���ւ̃x�N�g�������߂�B
	Vector3 add = Vector3(nextPosition - transform->GetPosition());
	//�J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
	// �R���W�����̒��S����R���C�_�[�T�C�Y���̍������Z�o�B
	//float Offset;
	// ���̍��W�ւ̌����x�N�g���B
	add.Normalize();
	//Offset = acosf(add.Dot(_halfSize));	// �����x�N�g���ɃT�C�Y���ˉe�B
	Vector3 posTmp = transform->GetPosition();
	//posTmp += add * Offset;
	Vector3 nextTmp = nextPosition;
	//nextTmp += add * Offset;

#ifdef TEST_CHARACON_XZ

	//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
	{
		int loopCount = 0;
		while (true) {
			Vector3 NowToNext(nextPosition - transform->GetPosition());
			NowToNext.y = 0.0f;
			if (NowToNext.Length() /*<*/ > FLT_EPSILON) {
				//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
				//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
				//�ƂĂ��������l�̂��Ƃł��B
				//���C���쐬�B
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓J�v�Z���R���C�_�[�̒��S���W�ɃT�C�Y�������Z�����n�_�B
				// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
				start.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, m_rigidBody->GetOffsetPos().y + 0.2f, m_rigidBody->GetOffsetPos().z));
				// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
				end.setOrigin(btVector3(nextPosition.x + m_rigidBody->GetOffset().x, m_rigidBody->GetOffsetPos().y + 0.2f, nextPosition.z + m_rigidBody->GetOffset().z));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = m_rigidBody->GetCollisionObj();
				callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
				callback._attribute = m_attributeXZ;
				//�Փˌ��o�B
				PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

				if (callback.isHit) {
					//���������B
					//�ǁB
					{
						Vector3 vT0, vT1;
						//XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ肷��B
						vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
						vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
						//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
						Vector3 vMerikomi;
						vMerikomi.Subtract(vT0, vT1);
						//XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
						Vector3 hitNormalXZ = callback.hitNormal;
						hitNormalXZ.y = 0.0f;
						hitNormalXZ.Normalize();
						//�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
						float fT0 = hitNormalXZ.Dot(vMerikomi);
						//�����߂��Ԃ��x�N�g�������߂�B
						//�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
						Vector3 vOffset;
						vOffset = hitNormalXZ;
						vOffset.Scale(-fT0 + _halfSize.x);
						nextPosition.Add(vOffset);
					}

					{
						//Vector3 vT0, vT1;
						////XZ���ʏ�ł̈ړ���̍��W��vT0�ɁA��_�̍��W��vT1�ɐݒ肷��B
						//vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
						//vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
						////�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
						//Vector3 vMerikomi;
						//vMerikomi = vT0 - vT1;
						////XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
						//Vector3 hitNormalXZ = callback.hitNormal;
						//hitNormalXZ.y = 0.0f;
						//hitNormalXZ.Normalize();
						////�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
						//float fT0 = fabsf(hitNormalXZ.Dot(vMerikomi));
						////�����߂��Ԃ��x�N�g�������߂�B
						////�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
						//Vector3 vOffset;
						//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
						//nextPosition.Add(vOffset);
					}
				}
				else {
					//�ǂ��Ƃ�������Ȃ��̂ŏI���B
					break;
				}
			}
			loopCount++;
			if (loopCount >= 5) {
				break;
			}
		}
	}
#endif

#ifdef TEST_CHARCON_Y

	////XZ�̈ړ��͊m��B
	//transform->SetPosition(Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));
	//�������𒲂ׂ�B
	{
		float addPosY = nextPosition.y - transform->GetPosition().y;

		transform->SetPosition(nextPosition.x, transform->GetPosition().y, nextPosition.z);	//�ړ��̉��m��B

		//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�͉��m�肵���ʒu�B
		start.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, /*posTmp*/m_rigidBody->GetOffsetPos().y/*transform->GetPosition().y - _halfSize.y + 0.2f*//* + m_height * 0.5f + m_radius*/, m_rigidBody->GetOffsetPos().z));
		//�I�_�͗�����̈ʒu�B
		end.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, nextPosition.y + m_rigidBody->GetOffset().y/* + m_height * 0.5f + m_radius*/, m_rigidBody->GetOffsetPos().z));
		
		Vector3 endPos;
		endPos.Set(end.getOrigin().x(), end.getOrigin().y(), end.getOrigin().z());
		if (m_isOnGround == false) {
			if (addPosY > 0.0f) {
				//�W�����v���Ƃ��ŏ㏸���B
				//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������̂ŉ��𒲂ׂ�B
				endPos.y -= addPosY * 0.01f;
			}
			else {
				//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
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
		//����0.0001�ȏ�Ȃ�Փˌ��o���܂�
		if (fabs(Sub.length()) > 0.0001f)
		{
			//�Փˌ��o�B
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;
			callback.SlipAngle = 54.0f;

			//int LoopCount = 0;
			//while (true) {
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B

				m_moveSpeed.y = 0.0f;
				//if (callback.isSlip) {
				//	// �Փ˖ʂɉ������x�N�g�����Z�o�B
				//	Vector3 normal = callback.hitNormal;
				//	Vector3 tangent = normal;
				//	normal.Cross(Vector3::up);
				//	tangent.Cross(normal);
				//	if (tangent.y > 0.0f) {
				//		tangent = tangent * -1.0f;
				//	}
				//	// �߂荞�ݗʎ擾�B
				//	float merikomi;
				//	merikomi = Vector3(callback.hitPos - nextPosition).Length();
				//	nextPosition.y = callback.hitPos.y;
				//	nextPosition += tangent * merikomi;

				//	m_isJump = true;
				//	m_isOnGround = false;
				//}
				//else {
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y + _halfSize.y - m_rigidBody->GetOffset().y;
				//}
				//break;
			//LoopCount++;
			//if (LoopCount >= 5) {
			//	break;
			//}
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B
				m_isOnGround = false;
				//break;
			}
		}
	}
#endif

	//�ړ��m��B
	transform->SetPosition(nextPosition);
	m_rigidBody->Update();
	//btRigidBody* btBody = (btRigidBody*)m_rigidBody->GetCollisonObj();
	//���̂𓮂����B
	m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);
	//btTransform& trans = btBody->getWorldTransform();
	////���̂̈ʒu���X�V�B
	//trans.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
	//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
}
