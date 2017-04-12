/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/

#include "CharacterController.h"
#include "CollisionAttr.h"
#include "Physics.h"

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
//struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
//{
//	bool isHit = false;									//�Փ˃t���O�B
//	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
//	Vector3 startPos = Vector3::zero;					//���C�̎n�_�B
//	Vector3 hitNormal = Vector3::zero;				//�Փ˓_�̖@���B
//	btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
//	float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
//
//														//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
//	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
//	{
//		if (convexResult.m_hitCollisionObject == me
//			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
//			) {
//			//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
//			return 0.0f;
//		}
//		//�Փ˓_�̖@�������������Ă���B
//		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
//		//������Ɩ@���̂Ȃ��p�x�����߂�B
//		float angle = hitNormalTmp.Dot(Vector3::up);
//		angle = fabsf(acosf(angle));
//		if (angle < 3.1415 * 0.3f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
//			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //�������̓R���W�����������n�ʂƎw�肳��Ă���B
//			) {
//			//�Փ˂��Ă���B
//			isHit = true;
//			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
//			//�Փ˓_�̋��������߂�B�B
//			Vector3 vDist;
//			vDist.Subtract(hitPosTmp, startPos);
//			float distTmp = vDist.Length();
//			if (dist > distTmp) {
//				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
//				hitPos = hitPosTmp;
//				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
//				dist = distTmp;
//			}
//		}
//		return 0.0f;
//	}
//};
//		//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
//struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
//{
//	bool isHit = false;						//�Փ˃t���O�B
//	Vector3 hitPos = Vector3::zero;		//�Փ˓_�B
//	Vector3 startPos = Vector3::zero;		//���C�̎n�_�B
//	float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
//	Vector3 hitNormal = Vector3::zero;	//�Փ˓_�̖@���B
//	btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
//											//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
//	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
//	{
//		if (convexResult.m_hitCollisionObject == me) {
//			//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
//			return 0.0f;
//		}
//		//�Փ˓_�̖@�������������Ă���B
//		Vector3 hitNormalTmp;
//		hitNormalTmp.Set(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
//		//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
//		float angle = fabsf(acosf(hitNormalTmp.Dot(Vector3::up)));
//		if (angle >= 3.1415 * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
//			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
//			) {
//			isHit = true;
//			Vector3 hitPosTmp;
//			hitPosTmp.Set(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
//			//��_�Ƃ̋����𒲂ׂ�B
//			Vector3 vDist;
//			vDist.Subtract(hitPosTmp, startPos);
//			vDist.y = 0.0f;
//			float distTmp = vDist.Length();
//			if (distTmp < dist) {
//				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
//				hitPos = hitPosTmp;
//				dist = distTmp;
//				hitNormal = hitNormalTmp;
//			}
//		}
//		return 0.0f;
//	}
//};



void CCharacterController::Init(GameObject* Object, Transform* tramsform, float radius, float height, Vector3 off, Collision_ID type, Collider* capsule, float gravity)
{
	//�R���W�����쐬�B
	m_radius = radius;
	m_height = height;
	//�d�͐ݒ�
	SetGravity(gravity);
	//m_collider->Create(radius, height);

	m_rigidBody.reset(new RigidBody(Object, tramsform));
	//���W�b�h�{�f�B�쐬
	m_rigidBody->Create(0.0f, capsule, type, Vector3::zero, off);
	//�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisonObj())->setSleepingThresholds(0, 0);


	m_collider = capsule;
	//���̂��������B
	//RigidBodyInfo rbInfo;
	//rbInfo.collider = m_collider;
	//rbInfo.mass = 0.0f;
	//m_rigidBody->Create(0.0f, m_collider, 5);
	//btTransform& trans = m_rigidBody->GetCollisonObj()->getWorldTransform();
	////���̂̈ʒu���X�V�B
	//trans.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
	////@todo ���Ή��Btrans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	//m_rigidBody->GetCollisonObj()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody->GetCollisonObj()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	//PhysicsWorld::Instance()->AddRigidBody(m_rigidBody);
}
void CCharacterController::Execute()
{
	//���x�ɏd�͉����x��������B
	m_moveSpeed.y += m_gravity * Time::DeltaTime();
	//���̈ړ���ƂȂ���W���v�Z����B
	Vector3 nextPosition = transform->GetLocalPosition();
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
			addPos.Subtract(nextPosition, transform->GetLocalPosition());
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() < FLT_EPSILON) {
				//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
				//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
				//�ƂĂ��������l�̂��Ƃł��B
				break;
			}
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

			SweepResultWall callback;
			callback.me = m_rigidBody->GetCollisonObj();
			callback.startPos = posTmp;
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
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}
	}
	//XZ�̈ړ��͊m��B
	transform->SetLocalPosition(Vector3(nextPosition.x, transform->GetLocalPosition().y, nextPosition.x));
	//�������𒲂ׂ�B
	{
		Vector3 addPos;
		addPos.Subtract(nextPosition, transform->GetLocalPosition());

		transform->SetLocalPosition(nextPosition);	//�ړ��̉��m��B
											//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
		start.setOrigin(btVector3(transform->GetLocalPosition().x, transform->GetLocalPosition().y + m_height * 0.5f + m_radius, transform->GetLocalPosition().z));
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
		SweepResultGround callback;
		callback.me = m_rigidBody->GetCollisonObj();
		callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		//�X�^�[�g�ƃG���h�̍�
		btVector3 Sub = start.getOrigin() - end.getOrigin();
		//����0.0001�ȏ�Ȃ�Փˌ��o���܂�
		if (fabs(Sub.length()) > 0.0001f)
		{
			//�Փˌ��o�B
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B
				//���������B
				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B
				m_isOnGround = false;
			}
		}
	}
	//�ړ��m��B
	transform->SetLocalPosition(nextPosition);
	btRigidBody* btBody = (btRigidBody*)m_rigidBody->GetCollisonObj();
	//���̂𓮂����B
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//���̂̈ʒu���X�V�B
	trans.setOrigin(btVector3(transform->GetLocalPosition().x, transform->GetLocalPosition().y, transform->GetLocalPosition().z));
	//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
}
/*!
* @brief	���S�������Ƃ�ʒm�B
*/
void CCharacterController::RemoveRigidBoby()
{
	PhysicsWorld::Instance()->RemoveRigidBody(m_rigidBody.get());
}