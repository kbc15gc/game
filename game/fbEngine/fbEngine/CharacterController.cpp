/*!
* @brief	�L�����N�^�̃R���W�����R���g���[���B
*/
#include"fbstdafx.h"
#include "CharacterController.h"
#include "_Object\_Component\_Physics\Collider.h"

#define TEST_CHARACON_XZ
#define TEST_CHARCON_Y


void CCharacterController::Init(Vector3 off, int type, Collider* coll, float gravity, int attributeXZ, int attributeY, bool isAddWorld)
{
	//�R���W�����쐬�B
	//�d�͐ݒ�B
	SetGravity(gravity);

	// �Փ˂����o������������ݒ�B
	m_attributeXZ = attributeXZ;
	m_attributeY = attributeY;

	//// �S�[�X�g�͓�����Ȃ��悤�ɐݒ�B
	//SubAttributeXZ(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));
	//SubAttributeY(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));

	m_rigidBody = gameObject->AddComponent<RigidBody>();
	//���W�b�h�{�f�B�쐬
	m_rigidBody->Create(0.0f, coll, type, Vector3::zero, off,Collision::PhysicsType::Kinematick,isAddWorld);
	//�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj())->setSleepingThresholds(0, 0);


	m_collider = coll;

	btVector3 localScaling(1.0f,1.0f,1.0f); // Transform�̃X�P�[���l�B
	localScaling = m_collider->GetBody()->getLocalScaling();
	_halfSize = m_rigidBody->GetShape()->GetHalfSize();
	//_halfSize.x *= localScaling.getX();
	//_halfSize.y *= localScaling.getY();
	//_halfSize.z *= localScaling.getZ();

	//���Ƃ��Ƃ̃t���O���c�����܂ܐV�����t���O��ǉ��B
	m_rigidBody->GetCollisionObj()->setCollisionFlags(m_rigidBody->GetCollisionObj()->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
}

const Vector3& CCharacterController::Execute()
{
	if (GetEnable() == false)
		return Vector3::zero;
	// �S�[�X�g�͓�����Ȃ��悤�ɐݒ�B
	SubAttributeXZ(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));
	SubAttributeY(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));

	_isHillThrough = false;

	float deltaTime = Time::DeltaTime();

	if (!_isHillThrough) {
		// �⓹��o��Ƃ��ɒ�R���󂯂�B
		_isOnWall = false;

		// �d�͂ɂ�錻�݂̑��x���X�V�B
		_nowGravitySpeed += (m_gravity * deltaTime);

		Vector3 nowPosTmp;
		Vector3 nextPosTmp;
		Vector3 originalXZDir;
		//���̈ړ���ƂȂ���W���v�Z����B
		{
			nextPosTmp = nowPosTmp = m_rigidBody->GetOffsetPos();	// �ړ��O�̃R���W�����̍��W(���_)��ݒ�B

			nextPosTmp.y = nowPosTmp.y = nowPosTmp.y - m_rigidBody->GetShape()->GetHalfSize().y;	// �ʒu�����R���W�����̑����ɍ��킹��B
			//�L�����N�^�[�̈ړ��ʂƊO�I�v���ɂ��ړ��ʂ����Z�B
			Vector3 addPos;
			addPos = (m_moveSpeed + _outsideSpeed) * deltaTime;
			// y�����͏d�͂����Z�B
			addPos.y += _nowGravitySpeed;

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
				//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
				// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
				start.setOrigin(btVector3(nowPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nowPosTmp.z));
				start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
				// ������Ȓn�ʂɈ���������Ȃ��悤�����グ��B
				end.setOrigin(btVector3(nextPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nextPosTmp.z));
				end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = gameObject;
				callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
				callback._attribute = m_attributeXZ;
				//�Փˌ��o�B
				INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

				if (callback.isHit/* && callback.hitID != static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST)*/) {
					//���������B
					// ���S�[�X�g�ł͂Ȃ��B

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

					_isOnWall = true;
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
			// Y�����݂̂̈ړ��ʁB
			Vector3 addPos = nextPosTmp - nowPosTmp;

			//���C���쐬����B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();

			// �J�n�ʒu�Ƒ����̍����B
			float startOffset;

			if (_isOnWall) {
				// �ǂɉ����߂��ꂽ�B

				//�n�_�͉��m�肵���J�v�Z���R���C�_�[�̒��S�ʒu(Y�����͈ړ��O)�B
				startOffset = m_rigidBody->GetShape()->GetHalfSize().y;
				start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + startOffset, nextPosTmp.z));
			}
			else {
				// �ǂɂ͓������Ă��Ȃ��B

				//�n�_�͏�������ꍇ���l�����ĉ��m�肵���J�v�Z���R���C�_�[�̒��S�ʒu���ォ��B
				startOffset = m_rigidBody->GetShape()->GetHalfSize().y * 2.0f;
				start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + startOffset, nextPosTmp.z));
			}

			start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

			//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
			//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
			//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
			// �����̏����͍⓹�̏����ŕK�v�B
			Vector3 endPos;
			endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//�W�����v���Ȃǂŏ㏸���B
					//�㏸���ł�XZ�Ɉړ��������ʂ߂荞��ł���\��������(����Ȃ�)�̂ŁA�ړ���̍��W���牺�𒲂ׂ�B
					//endPos.y -= addPos.y * 0.01f;
					start.getOrigin().setY(start.getOrigin().getY() + addPos.y);
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					////�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
					//endPos.y += addPos.y;

					//������l�����ĊJ�n�ʒu�����߂ɂ��Ă���̂ŁA�I���ʒu�͂��̍�������������B
					endPos.y += (addPos.y - startOffset);
				}
			}
			else {
				//�n�ʏ�ɂ���B
				
				//// 1m��������B
				//// �������~���Ƃ��ɃK�^�K�^���Ȃ��悤�B
				//endPos.y -= 1.0f;

				//�������Ă���B

				//������l�����ĊJ�n�ʒu�����߂ɂ��Ă���̂ŁA�I���ʒu�͂��̍�������������B
				// 1m��������͉̂������~���Ƃ��ɃK�^�K�^���Ȃ��悤�B
				endPos.y -= (1.0f + startOffset);
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
			////�X�^�[�g�ƃG���h�̍�
			//btVector3 Sub = start.getOrigin() - end.getOrigin();
			////����0.0001�ȏ�Ȃ�Փˌ��o����B
			//if (fabs(Sub.length()) > 0.0001f)
			//{
				//�Փˌ��o�B
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = gameObject;
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;

			INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//�n�ʂɐڐG���Ă���B

				//if (callback.hitID == static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST)) {
				//	// �S�[�X�g�͖����B
				//}
				//else {
					//m_moveSpeed.y = 0.0f;	// �n�ʂɓ��������̂ňړ��ʂ�0�ɂ���B
					m_isJump = false;
					m_isOnGround = true;
					_nowGravitySpeed = 0.0f;	// �n�ʂɓ������Ă���̂ŏd�͂ɂ��X�s�[�h�𖳌��ɂ���B
					nextPosTmp.y = callback.hitPos.y;	// �Փ˓_�͑����B
				//}
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B

				m_isOnGround = false;
			}
			//}
		}
#endif

		nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// �����̈ʒu�ɐݒ肵�Ă����̂ŃR���W�����̒��S�ɖ߂��B
		//�ړ��m��B
		m_rigidBody->SubOffset(nextPosTmp);	// �R���W�����̒��S���W�Ȃ̂Ń��f���̌��_�܂ō����̕����Z�B

		_moveSpeedExcute = nextPosTmp - transform->GetPosition();	// ���ۂ̈ړ��ʂ�ۑ��B

		transform->SetPosition(nextPosTmp);
		m_rigidBody->Update();	// transform���X�V�����̂ŃR���W������Transform���X�V����B
		//���̂𓮂����B
		m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);

		_outsideSpeed = Vector3::zero;

		_moveSpeedExcute = _moveSpeedExcute / deltaTime;
	}
	else {
		// �⓹�����邷��o��B

		float radius = 0.2f;
		float height = 1.3f;

		// �d�͂ɂ�錻�݂̑��x���X�V�B
		_nowGravitySpeed += (m_gravity * deltaTime);

		//���x�ɏd�͉����x��������B
		m_moveSpeed.y += _nowGravitySpeed;
		//���̈ړ���ƂȂ���W���v�Z����B
		Vector3 nextPosition = transform->GetPosition();
		//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
		Vector3 addPos = m_moveSpeed;
		addPos.Scale(deltaTime);
		nextPosition.Add(addPos);
		Vector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ���ʂł̏Փˌ��o�ƏՓˉ������s���B
		{
			int loopCount = 0;
			while (true) {
				//���݂̍��W���玟�̈ړ���֌������x�N�g�������߂�B
				Vector3 addPos;
				addPos.Subtract(nextPosition, transform->GetPosition());
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
				posTmp.y += radius + height * 0.5f + 0.2f;
				//���C���쐬�B
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓J�v�Z���R���C�_�[�̒��S���W + 0.2�̍��W��posTmp�ɋ��߂�B
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//�I�_�͎��̈ړ���BXZ���ʂł̏Փ˂𒲂ׂ�̂ŁAy��posTmp.y��ݒ肷��B
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = gameObject;
				callback.startPos = posTmp;
				//�Փˌ��o�B
				INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

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
					vOffset.Scale(-fT0 + radius);
					nextPosition.Add(vOffset);
					Vector3 currentDir;
					currentDir.Subtract(nextPosition, transform->GetPosition());
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//�p�ɓ��������̃L�����N�^�̐U����h�~���邽�߂ɁA
						//�ړ��悪�t�����ɂȂ�����ړ����L�����Z������B
						nextPosition.x = transform->GetPosition().x;
						nextPosition.z = transform->GetPosition().z;
						break;
					}
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
		transform->SetPosition(Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));
		//�������𒲂ׂ�B
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, transform->GetPosition());

			transform->SetPosition(nextPosition);	//�ړ��̉��m��B
										//���C���쐬����B
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
			start.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y + radius + height * 0.5f, transform->GetPosition().z));
			//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
			//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
			//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
			Vector3 endPos;
			endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
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
			callback.me = gameObject;
			callback.startPos.Set(Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ()));
			//�Փˌ��o�B
			INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B
				_nowGravitySpeed = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;
			}
			else {
				//�n�ʏ�ɂ��Ȃ��B
				m_isOnGround = false;
			}
		}
		//�ړ��m��B
		transform->SetPosition(nextPosition);
		//btRigidBody* btBody = static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj());
		////���̂𓮂����B
		//btBody->setActivationState(DISABLE_DEACTIVATION);
		//btTransform& trans = btBody->getWorldTransform();
		////���̂̈ʒu���X�V�B
		//trans.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
	}

	return _moveSpeedExcute;
}

/**
* ������n�ʂɍ��킹��.
*/
void CCharacterController::FitGround()
{
	//���C���쐬����.
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//�J�n�ʒu�Ƒ����̍���.
	float startOffset = 2;

	Vector3 pos = transform->GetPosition();
	//�J�n�n�_��ݒ�.
	start.setOrigin(btVector3(pos.x, pos.y + startOffset, pos.z));

	//�I���n�_��ݒ�.
	//2���[�g����������.
	end.setOrigin(start.getOrigin() - btVector3(0, startOffset + 2, 0));

	fbPhysicsCallback::SweepResultGround callback;
	callback.me = gameObject;
	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
	callback._attribute = m_attributeY;

	INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

	if (callback.isHit)
	{
		pos = callback.hitPos;
		pos.y += (m_rigidBody->GetShape()->GetHalfSize().y) * transform->GetScale().y;	// �����̈ʒu�ɐݒ肵�Ă����̂ŃR���W�����̒��S�ɖ߂��B
																	//�ړ��m��B
		m_rigidBody->SubOffset(pos);	// �R���W�����̒��S���W�Ȃ̂Ń��f���̌��_�܂ō����̕����Z�B

		transform->SetPosition(pos);
		m_rigidBody->Update();	// transform���X�V�����̂ŃR���W������Transform���X�V����B
	}
}
