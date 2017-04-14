#pragma once
#include "Collision.h"
#include "../../Decide/Decide/GameSystem.h"
struct MyContactResultCallback : public btCollisionWorld::ContactResultCallback
{
public:
	MyContactResultCallback()
	{

	}
	//�Փ˂��ꂽ���̃R�[���o�b�N�֐�
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		//������
		const Vector3* vColl0Pos = (Vector3*)(&colObj0Wrap->getWorldTransform().getOrigin());
		const Vector3* vColl1Pos = (Vector3*)(&colObj1Wrap->getWorldTransform().getOrigin());
		Vector3 vDist;
		vDist.Subtract(*vColl0Pos, *vColl1Pos);
		float distTmpSq = vDist.LengthSq();
		Collision* hitObjectTmp;
		//�߂��Ȃ�
		if (distTmpSq < distSq) {
			//������̕����߂��B
			if (queryCollisionObject->GetCollisonObj() == colObj0Wrap->getCollisionObject()) {
				hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
			}
			else {
				hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
			}
			//hit�I�u�W�F�N�g������ && �R���W�������w�肵��ID�ł���B
			if (hitObjectTmp && id == hitObjectTmp->GetCollisonObj()->getUserIndex()) {
				distSq = distTmpSq;
				hitObject = hitObjectTmp;
				hitObject->SetHit(true);
				queryCollisionObject->SetHit(true);
			}
		}

		return 0.0f;
	}
public:
	int id;												//��������R���W����ID
	float distSq = FLT_MAX;								//������ێ�
	Collision* hitObject = nullptr;						//�q�b�g�����I�u�W�F�N�g
	Collision* queryCollisionObject = nullptr;			//���g�H
};

//���O�Ō�������R�[���o�b�N
struct FindNameContactResultCallback : public btCollisionWorld::ContactResultCallback
{
public:
	FindNameContactResultCallback()
	{

	}
	//�Փ˂��ꂽ���̃R�[���o�b�N�֐�
	virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
	{
		//�q�b�g�����R���W�������i�[����B
		Collision* hitObjectTmp;

		//���g�ł�����?�悭�킩��Ȃ��B
		if (queryCollisionObject->GetCollisonObj() == colObj0Wrap->getCollisionObject())
		{
			hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
		}
		else
		{
			hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
		}
		//�R���W����ID�擾
		int hitid = hitObjectTmp->GetCollisonObj()->getUserIndex();
		//�R���W�����l�[���擾
		const char* hitname = hitObjectTmp->gameObject->GetName();
		if (hitObjectTmp &&				//hit�I�u�W�F�N�g������
			id == hitid &&				//�R���W�������w�肵��ID�ł���B
			strcmp(name, hitname) == 0)	//�w�肵�����O�Ɠ����Q�[���I�u�W�F�N�g�ł���B
		{
			hitObject = hitObjectTmp;
			hitObject->SetHit(true);
			queryCollisionObject->SetHit(true);
		}

		return 0.0f;
	}
public:
	int id;												//��������R���W����ID
	const char* name;									//��������R���W�����l�[��
	float distSq = FLT_MAX;								//������ێ�
	Collision* hitObject = nullptr;						//�q�b�g�����I�u�W�F�N�g
	Collision* queryCollisionObject = nullptr;			//���g�H
};

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;									//�Փ˃t���O�B
	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
	Vector3 startPos = Vector3::zero;					//���C�̎n�_�B
	Vector3 hitNormal = Vector3::zero;				//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
	float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B

														//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == Collision_ID::PLAYER
			//|| convexResult.m_hitCollisionObject->getUserIndex() == Collision_ID::
			) {
			//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
		//������Ɩ@���̂Ȃ��p�x�����߂�B
		float angle = hitNormalTmp.Dot(Vector3::up);
		angle = fabsf(acosf(angle));
		if (angle < D3DX_PI * 0.2f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == Collision_ID::GROUND //�������̓R���W�����������n�ʂƎw�肳��Ă���B
			) {
			//�Փ˂��Ă���B
			isHit = true;

			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//�Փ˓_�̋��������߂�B�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}
		return 0.0f;
	}
};

struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	Vector3 hitPos = Vector3::zero;		//�Փ˓_�B
	Vector3 startPos = Vector3::zero;		//���C�̎n�_�B
	float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
	Vector3 hitNormal = Vector3::zero;	//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
											//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me) {
			//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp;
		hitNormalTmp.Set(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
		//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
		float angle = fabsf(acosf(hitNormalTmp.Dot(Vector3::up)));
		if (angle >= 3.1415 * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == Collision_ID::PLAYER	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
			) {
			isHit = true;
			Vector3 hitPosTmp;
			hitPosTmp.Set(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
			//��_�Ƃ̋����𒲂ׂ�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}
		return 0.0f;
	}
};