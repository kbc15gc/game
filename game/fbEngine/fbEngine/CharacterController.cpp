/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/
#include"fbstdafx.h"
#include "CharacterController.h"
#include "_Object\_Component\_Physics\Collider.h"

void CCharacterController::Init(GameObject* Object, Transform* tramsform, float radius, float height, Vector3 off, int type, Collider* capsule, float gravity, int attributeXZ, int attributeY, bool isAddWorld)
{
	//�R���W�����쐬�B
	m_radius = radius;
	m_height = height;
	//�d�͐ݒ�B
	SetGravity(gravity);

	// �Փ˂����o������������ݒ�B
	m_attributeXZ = attributeXZ;
	m_attributeY = attributeY;

	m_rigidBody = gameObject->AddComponent<RigidBody>();
	//���W�b�h�{�f�B�쐬
	m_rigidBody->Create(0.0f, capsule, type, Vector3::zero, off,isAddWorld);
	//�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisonObj())->setSleepingThresholds(0, 0);


	m_collider = capsule;
	//���Ƃ��Ƃ̃t���O���c�����܂ܐV�����t���O��ǉ��B
	m_rigidBody->GetCollisonObj()->setCollisionFlags(m_rigidBody->GetCollisonObj()->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
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

	//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
	{
		int loopCount = 0;
		while (true) {
			//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
			Vector3 addPos;
			addPos.Subtract(nextPosition, transform->GetPosition());
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() /*<*/ > FLT_EPSILON) {
				//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
				//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
				//�ƂĂ��������l�̂��Ƃł��B
			//	break;
			//}
			//�J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				Vector3 posTmp = transform->GetPosition();
				posTmp.y += m_height * 0.5f + m_radius + 0.2f;
				//���C���쐬�B
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ�̂ŁAy��posTmp.y��ݒ肷��B
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = m_rigidBody->GetCollisonObj();
				callback.startPos = posTmp;
				callback._attribute = m_attributeXZ;
				//�Փˌ��o�B
				PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

				if (callback.isHit) {
					//���������B
					//�ǁB

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
					vOffset.Scale(-fT0 + m_radius);
					nextPosition.Add(vOffset);
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
	////XZ�̈ړ��͊m��B
	//transform->SetPosition(Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));
	//�������𒲂ׂ�B
	{
		Vector3 addPos;
		addPos.Subtract(nextPosition, Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));

		transform->SetPosition(nextPosition);	//�ړ��̉��m��B

		//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
		start.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y + m_height * 0.5f + m_radius, transform->GetPosition().z));
		//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
		//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
		//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
		Vector3 endPos;
		endPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//�W�����v���Ƃ��ŏ㏸���B
				//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������̂ŉ��𒲂ׂ�B
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
				endPos.y += addPos.y;
			}
		}
		else {
			//�n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		fbPhysicsCallback::SweepResultGround callback;
		callback.me = m_rigidBody->GetCollisonObj();
		callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		callback._attribute = m_attributeY;
		//�X�^�[�g�ƃG���h�̍�
		btVector3 Sub = start.getOrigin() - end.getOrigin();
		//����0.0001�ȏ�Ȃ�Փˌ��o���܂�
		if (fabs(Sub.length()) > 0.0001f)
		{
			//�Փˌ��o�B
			//int LoopCount = 0;
			//while (true) {
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B

				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;

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
	//�ړ��m��B
	transform->SetPosition(nextPosition);
	m_rigidBody->Update();
	//btRigidBody* btBody = (btRigidBody*)m_rigidBody->GetCollisonObj();
	//���̂𓮂����B
	m_rigidBody->GetCollisonObj()->setActivationState(DISABLE_DEACTIVATION);
	//btTransform& trans = btBody->getWorldTransform();
	////���̂̈ʒu���X�V�B
	//trans.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
	//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
}
