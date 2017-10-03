#include "fbstdafx.h"
#include "_Object\_Component\_Physics\CharacterExtrude.h"
#include "CharacterController.h"

void CharacterExtrude::Init(RigidBody* collision, int attributeXZ, int attributeY) {
	// �Փ˂����o������������ݒ�B
	_attributeXZ = attributeXZ;
	_attributeY = attributeY;

	_collision = collision;

	btVector3 localScaling(1.0f, 1.0f, 1.0f); // Transform�̃X�P�[���l�B
	localScaling = _collision->GetShape()->GetBody()->getLocalScaling();
	_halfSize = _collision->GetShape()->GetHalfSize();
	_halfSize.x *= localScaling.getX();
	_halfSize.y *= localScaling.getY();
	_halfSize.z *= localScaling.getZ();
}

void CharacterExtrude::Extrude(const Vector3& speed) {
	Vector3 startPos;
	Vector3 endPos;
	startPos = endPos = _collision->GetOffsetPos();	// �ړ��O�̃R���W�����̍��W(���_)��ݒ�B
	startPos -= speed;	// �X�^�[�g�ʒu���ړ��O�̈ʒu�ɐݒ�B

	//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
	{
		Vector3 NowToNext(endPos - startPos);
		NowToNext.y = 0.0f;
		if (NowToNext.Length() <= FLT_EPSILON) {
			//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
			//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
			//�ƂĂ��������l�̂��Ƃł��B

		}
		else {
			//���C���쐬�B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
			// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
			start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
			// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

			fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
			//�Փˌ��o�B
			PhysicsWorld::Instance()->ConvexSweepTest(static_cast<const btConvexShape*>(_collision->GetShape()->GetBody()), start, end, callback);

			if (callback.isHit) {
				//���������B

				vector<fbPhysicsCallback::SweepResultExtrude::hitInfo*> infoArray = callback.GetInfoArray();
				for (int idx = 0; idx < infoArray.size(); idx++) {
					Vector3 vT0, vT1;
					//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
					Vector3 vMerikomi;
					vMerikomi = (endPos - startPos) * (1.0f - infoArray[idx]->hitFraction);
					vMerikomi.y = 0.0f;

					// �e�X�g�B
					CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
					if (CC) {
						CC->AddOutsideSpeed(vMerikomi);
					}

					////XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
					//Vector3 hitNormalXZ = callback.hitNormal;
					//hitNormalXZ.y = 0.0f;
					//hitNormalXZ.Normalize();
					////�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
					//float fT0 = hitNormalXZ.Dot(vMerikomi);
					////�����߂��Ԃ��x�N�g�������߂�B
					////�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
					//Vector3 vOffset;
					//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
					//nextPosTmp += vOffset;
				}
			}
		}
	}

	////�������𒲂ׂ�B
	//{

	//	Vector3 NowToNext(endPos - startPos);
	//	NowToNext.x = 0.0f;
	//	NowToNext.z = 0.0f;
	//	if (NowToNext.Length() <= FLT_EPSILON) {
	//		//Y���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
	//		//FLT_EPSILON��1���傫���A�ŏ��̒l�Ƃ̍�����\���萔�B
	//		//�ƂĂ��������l�̂��Ƃł��B

	//	}
	//	else {
	//		//���C���쐬�B
	//		btTransform start, end;
	//		start.setIdentity();
	//		end.setIdentity();
	//		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
	//		// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
	//		start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
	//		// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
	//		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

	//		fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
	//		//�Փˌ��o�B
	//		PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)_collision->GetCollisionObj(), start, end, callback);

	//		if (callback.isHit) {
	//			//���������B

	//			vector<unique_ptr<fbPhysicsCallback::SweepResultExtrude::hitInfo>> infoArray = callback.GetInfoArray();
	//			for (int idx = 0; idx < infoArray.size(); idx++) {
	//				Vector3 vT0, vT1;
	//				//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
	//				Vector3 vMerikomi;
	//				vMerikomi = (endPos - startPos) * (1.0f - infoArray[idx]->hitFraction);

	//				// �e�X�g�B
	//				CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
	//				if (CC) {
	//					CC->AddOutsideSpeed(vMerikomi);
	//				}

	//				////XZ���ʂł̏Փ˂����ǂ̖@�������߂�B�B
	//				//Vector3 hitNormalXZ = callback.hitNormal;
	//				//hitNormalXZ.y = 0.0f;
	//				//hitNormalXZ.Normalize();
	//				////�߂荞�݃x�N�g����ǂ̖@���Ɏˉe����B
	//				//float fT0 = hitNormalXZ.Dot(vMerikomi);
	//				////�����߂��Ԃ��x�N�g�������߂�B
	//				////�����Ԃ��x�N�g���͕ǂ̖@���Ɏˉe���ꂽ�߂荞�݃x�N�g��+���a�B
	//				//Vector3 vOffset;
	//				//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
	//				//nextPosTmp += vOffset;
	//			}
	//		}
	//	}


		//Vector3 addPos = nextPosTmp - nowPosTmp;

		////���C���쐬����B
		//btTransform start, end;
		//start.setIdentity();
		//end.setIdentity();
		////�n�_�͉��m�肵���J�v�Z���R���C�_�[�̒��S�ʒu(Y�����͈ړ��O)�B
		//start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y, nextPosTmp.z));
		////�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
		////�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
		////�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
		//// �����̏����͍⓹�̏����ŕK�v�B
		//Vector3 endPos;
		//endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
		//if (m_isOnGround == false) {
		//	if (addPos.y > 0.0f) {
		//		//�W�����v���Ƃ��ŏ㏸���B
		//		//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������(����Ȃ�)�̂ŁA�ړ���̍��W���牺�𒲂ׂ�B
		//		endPos.y -= addPos.y * 0.01f;
		//	}
		//	else {
		//		//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
		//		endPos.y += addPos.y;
		//	}
		//}
		//else {
		//	//�n�ʏ�ɂ���ꍇ��1m��������B
		//	// �������~���Ƃ��ɃK�^�K�^���Ȃ��悤�B
		//	endPos.y -= 1.0f;
		//}
		//end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		////�X�^�[�g�ƃG���h�̍�
		//btVector3 Sub = start.getOrigin() - end.getOrigin();
		////����0.0001�ȏ�Ȃ�Փˌ��o����B
		//if (fabs(Sub.length()) > 0.0001f)
		//{
		//	//�Փˌ��o�B
		//	fbPhysicsCallback::SweepResultGround callback;
		//	callback.me = m_rigidBody->GetCollisionObj();
		//	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		//	callback._attribute = m_attributeY;

		//	PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
		//	if (callback.isHit) {
		//		//�n�ʂɐڐG���Ă���B

		//		if (gameObject == INSTANCE(GameObjectManager)->FindObject("Player")) {
		//			OutputDebugString("aa");
		//		}

		//		m_isJump = false;
		//		m_isOnGround = true;
		//		nextPosTmp.y = callback.hitPos.y;
		//	}
		//	else {
		//		//�n�ʏ�ɂ��Ȃ��B

		//		m_isOnGround = false;
		//	}
		//}
	//}

	//nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// �����̈ʒu�ɐݒ肵�Ă����̂ŃR���W�����̒��S�ɖ߂��B
	//															//�ړ��m��B
	//nextPosTmp -= m_rigidBody->GetOffset();
	//m_moveSpeed = nextPosTmp - transform->GetPosition();	// �Փˉ�����̍ŏI�I�Ȉړ��ʂ�ۑ��B
	//transform->SetPosition(nextPosTmp);	// nextPosTmp�̓��f���̌��_�ƃC�R�[���łȂ��\��������̂�Offset�����Z���Đݒ肷��B
	//m_rigidBody->Update();	// transform���X�V�����̂ŃR���W������Transfiorm���X�V����B
	//						//���̂𓮂����B
	//m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);

	//return m_moveSpeed;
}
