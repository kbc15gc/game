#include "fbstdafx.h"
#include "_Object\_Component\_Physics\CharacterExtrude.h"
#include "CharacterController.h"

void CharacterExtrude::Init(const vector<RigidBody*>& collisions, int attribute) {
	_collisions.clear();
	_halfSize.clear();

	// �����o������������ݒ�B
	_attribute = attribute;

	_collisions = collisions;

	for (auto coll : _collisions) {
		btVector3 localScaling(1.0f, 1.0f, 1.0f); // Transform�̃X�P�[���l�B
		localScaling = coll->GetShape()->GetBody()->getLocalScaling();
		Vector3 halfSize = coll->GetShape()->GetHalfSize();
		halfSize.x *= localScaling.getX();
		halfSize.y *= localScaling.getY();
		halfSize.z *= localScaling.getZ();

		_halfSize.push_back(halfSize);
	}
}

void CharacterExtrude::Extrude(const Vector3& speed) {

	// �X�E�B�[�v�e�X�g�B
	{
		//Vector3 startPos;
		//Vector3 endPos;
		//startPos = endPos = _collision->GetOffsetPos();	// �ړ��O�̃R���W�����̍��W(���_)��ݒ�B
		//startPos -= speed;	// �X�^�[�g�ʒu���ړ��O�̈ʒu�ɐݒ�B

		////XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
		//{
		//	Vector3 NowToNext(endPos - startPos);
		//	NowToNext.y = 0.0f;
		//	if (NowToNext.Length() <= FLT_EPSILON) {
		//		//XZ���ʂœ������Ȃ��̂Œ��ׂ�K�v�Ȃ��B
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
		//		start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
		//		// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
		//		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		//		end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

		//		fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
		//		//�Փˌ��o�B
		//		PhysicsWorld::Instance()->ConvexSweepTest(static_cast<const btConvexShape*>(_collision->GetShape()->GetBody()), start, end, callback);

		//		if (callback.isHit) {
		//			//���������B

		//			vector<fbPhysicsCallback::SweepResultExtrude::hitInfo*> infoArray = callback.GetInfoArray();
		//			for (int idx = 0; idx < infoArray.size(); idx++) {
		//				Vector3 vT0, vT1;

		//				// �Փ˂����|�C���g�����߂�B
		//				Vector3 hitPos = startPos + ((endPos - startPos) * infoArray[idx]->hitFraction);

		//				//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
		//				Vector3 vMerikomi;
		//				vMerikomi = endPos - hitPos;
		//				vMerikomi.y = 0.0f;

		//				//�����o�����������߂�B
		//				Vector3 hitNormalXZ = infoArray[idx]->hitPos - hitPos;
		//				hitNormalXZ.y = 0.0f;
		//				hitNormalXZ.Normalize();
		//				Vector3 dir = endPos - startPos;
		//				dir.y = 0.0f;
		//				dir.Normalize();
		//				float dot = hitNormalXZ.Dot(dir);
		//				if (hitNormalXZ.Dot(dir) < 0.0f) {
		//					// ���������R���W�������i�s�����̔��Α��ɂ���B

		//					// �i�ޕ����ɉ����o���̂Ŗ����B
		//					return;
		//				}

		//				//�߂荞�݃x�N�g����@���Ɏˉe����B
		//				float fT0 = hitNormalXZ.Dot(vMerikomi);
		//				//�����Ԃ��x�N�g�������߂�B
		//				//�����Ԃ��x�N�g���͖@���Ɏˉe���ꂽ�߂荞�݃x�N�g���B
		//				Vector3 vOffset;
		//				vOffset = hitNormalXZ * fT0;
		//				vOffset = vOffset / Time::DeltaTime();

		//				// �Փ˂����R���W�����ɗ͂�������B
		//				CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
		//				if (CC) {
		//					CC->AddOutsideSpeed(vOffset);
		//				}
		//			}
		//		}
		//	}
		//}
	}


	// �R���^�N�g�e�X�g�B
	{
		for (auto coll : _collisions) {
			fbPhysicsCallback::AllHitsContactResultCallback callback;
			vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> infoArray;
			//�Փˌ��o�B
			PhysicsWorld::Instance()->AllHitsContactTest(coll, &infoArray, &callback, _attribute);

			for (int idx = 0; idx < static_cast<int>(infoArray.size()); idx++) {
				Vector3 vT0, vT1;

				//�߂荞�݂��������Ă���ړ��x�N�g�������߂�B
				Vector3 vMerikomi;
				vMerikomi = infoArray[idx]->hitPosB - infoArray[idx]->hitPosA;
				vMerikomi.y = 0.0f;

				//�����o�����������߂�B
				Vector3 hitNormalXZ = infoArray[idx]->hitPosA - coll->GetOffsetPos();
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();

				//�߂荞�݃x�N�g����@���Ɏˉe����B
				float fT0 = hitNormalXZ.Dot(vMerikomi);

				if (fabsf(fT0) <= 0.0f) {
					// Y�����݂̂̂߂荞�݂������B

					// ��O�����B
					// �Ƃ肠�����O�����ɉ����o���B
					hitNormalXZ = transform->GetForward();

					// �����o���ʂ��Z�o�B
					Vector3 pos1 = coll->GetOffsetPos();
					Vector3 pos2 = infoArray[idx]->collision->GetOffsetPos();
					pos1.y = 0.0f;
					pos2.y = 0.0f;
					Vector3 work = pos2 - pos1;
					float fMerikomi = hitNormalXZ.Dot(work);
					fT0 = -(coll->GetShape()->GetHalfSize().z - fMerikomi);
				}

				//�����Ԃ��x�N�g�������߂�B
				//�����Ԃ��x�N�g���͖@���Ɏˉe���ꂽ�߂荞�݃x�N�g���B
				Vector3 vOffset;
				vOffset = hitNormalXZ * -fT0;
				vOffset = vOffset / Time::DeltaTime();

				// �Փ˂����R���W�����ɗ͂�������B
				CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
				if (CC) {
					CC->AddOutsideSpeed(vOffset);
				}
			}
		}
	}
}
