#pragma once
#include "_Include\CollisionInclude.h"

namespace fbPhysicsCallback
{
	//�S�[�X�g�Əd�Ȃ��Ă���y�A��T���R�[���o�b�N
	struct MyGhostPairCallback : public btGhostPairCallback
	{
	public:
		//�d�Ȃ����Ƃ��ɌĂ΂��֐�
		btBroadphasePair*	addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1)
		{
			btCollisionObject* colObj0 = (btCollisionObject*)proxy0->m_clientObject;
			btCollisionObject* colObj1 = (btCollisionObject*)proxy1->m_clientObject;
			//�S�[�X�g�I�u�W�F�N�g�ɃA�b�v�L���X�g
			//(�����S�[�X�g�I�u�W�F�N�g�ȊO�Ȃ�null�ɂȂ�)
			btGhostObject* ghost0 = btGhostObject::upcast(colObj0);
			btGhostObject* ghost1 = btGhostObject::upcast(colObj1);
			Collision* coll0 = (Collision*)colObj0->getUserPointer();
			Collision* coll1 = (Collision*)colObj1->getUserPointer();

			if (ghost0)
			{
				//�y�A�ǉ�
				ghost0->addOverlappingObjectInternal(proxy1, proxy0);
			}
			if (ghost1)
			{
				//�y�A�ǉ�
				ghost1->addOverlappingObjectInternal(proxy0, proxy1);				
			}
			return 0;
		}
		//�d�Ȃ肩�甲�������ɌĂ΂��֐�
		void*	removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher)
		{
			btCollisionObject* colObj0 = (btCollisionObject*)proxy0->m_clientObject;
			btCollisionObject* colObj1 = (btCollisionObject*)proxy1->m_clientObject;
			//�S�[�X�g�I�u�W�F�N�g�ɃA�b�v�L���X�g
			//(�����S�[�X�g�I�u�W�F�N�g�ȊO�Ȃ�null�ɂȂ�)
			btGhostObject* ghost0 = btGhostObject::upcast(colObj0);
			btGhostObject* ghost1 = btGhostObject::upcast(colObj1);
			if (ghost0)
			{
				ghost0->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
			}
			if (ghost1)
			{
				ghost1->removeOverlappingObjectInternal(proxy0, dispatcher, proxy1);
			}
			return 0;
		}
	};

	//�q�b�g�������ōł��߂����̂�Ԃ��R�[���o�b�N
	struct ClosestContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		ClosestContactResultCallback()
		{

		}
		//�Փ˂��ꂽ���̊֐�
		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{

			//�|�W�V�����擾
			const Vector3* vColl0Pos = (Vector3*)(&colObj0Wrap->getWorldTransform().getOrigin());
			const Vector3* vColl1Pos = (Vector3*)(&colObj1Wrap->getWorldTransform().getOrigin());
			Vector3 vDist;
			//���g�Ɠ��������I�u�W�F�N�g�Ƃ̋������v�Z
			vDist.Subtract(*vColl0Pos, *vColl1Pos);
			//����(2��̂܂܂Ȃ̂͌v�Z���Ȃ�����)
			float distTmpSq = vDist.LengthSq();
			//�ꎞ�I(temporary)�ɃR���W�������i�[����
			Collision* hitObjectTmp;
			//�߂��Ȃ�
			if (distTmpSq < distSq) {
				// col0 �� col1 �̂ǂ��炩�͎����Ȃ̂ŃA�h���X���r���Ċm���߂�
				if (me->GetCollisionObj() == colObj0Wrap->getCollisionObject()) {
					hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
				}
				else {
					hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
				}
				//hit�I�u�W�F�N�g������
				if (hitObjectTmp)
				{
					//��������v���邩�H�}�X�N���Ƃ�
					if ((attribute & hitObjectTmp->GetCollisionObj()->getUserIndex()) != 0)
					{
						//�������X�V
						distSq = distTmpSq;
						//�R���W�������X�V
						hitObject = hitObjectTmp;
					}
				}
			}

			return 0.0f;
		}
	public:
		int attribute;					//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�
		float distSq = FLT_MAX;			//������ێ�
		Collision* me = nullptr;		//���g�̃A�h���X
		Collision* hitObject = nullptr;	//�q�b�g�����I�u�W�F�N�g
	};

	//�q�b�g�������̑S�Ă�Ԃ��R�[���o�b�N
	struct AllHitsContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		AllHitsContactResultCallback()
		{

		}
		//�Փ˂��ꂽ���̊֐�
		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{
			//�ꎞ�I(temporary)�ɃR���W�������i�[����
			Collision* hitObjectTmp = nullptr;

			// col0 �� col1 �̂ǂ��炩�͎����Ȃ̂ŃA�h���X���r���Ċm���߂�
			if (me->GetCollisionObj() == colObj0Wrap->getCollisionObject()) {
				hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
			}
			else {
				hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
			}
			
			//hit�I�u�W�F�N�g������
			if (hitObjectTmp)
			{

				//��������v���邩�H�}�X�N���Ƃ�
				if ((attribute & hitObjectTmp->GetCollisionObj()->getUserIndex()) != 0)
				{
					//�d���`�F�b�N
					for each (Collision* coll in hitObjects)
					{
						//�A�h���X��r
						if (coll == hitObjectTmp)
						{
							return 0.0f;
						}
					}
					//�R���W������ǉ�
					hitObjects.push_back(hitObjectTmp);
				}
			}
			return 0.0f;
		}
	public:
		int attribute;					//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�
		Collision* me = nullptr;		//���g�̃A�h���X
		vector<Collision*> hitObjects;	//�q�b�g�����I�u�W�F�N�g
	};

	//���C���΂��ăq�b�g�������ōł��߂����̂�Ԃ��B
	struct	ClosestRayResultCallback : public btCollisionWorld::RayResultCallback
	{
	public:
		ClosestRayResultCallback(const btVector3& from, const btVector3& to, const int& attr)
		{
			_fromPos = from;
			_toPos = to;
			_attribute = attr;

			m_hitPointWorld = btVector3(0, 0, 0);
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if ((_attribute & rayResult.m_collisionObject->getUserIndex()) == 0)
			{
				return 0.0f;
			}
			//�ꎞ�I��
			float fracTmp = rayResult.m_hitFraction;
			//�߂��Ȃ�
			if (fracTmp < _Fraction)
			{
				//�n�_����I�_��0�`1�ɂ������̃q�b�g�����ʒu�̊����B
				_Fraction = m_closestHitFraction = fracTmp;
				//�q�b�g�����R���W����
				m_collisionObject = rayResult.m_collisionObject;
				if (normalInWorldSpace)
				{
					m_hitNormalWorld = rayResult.m_hitNormalLocal;
				}
				else
				{
					///need to Transform normal into worldspace
					m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
				}
				//�q�b�g�����ʒu���擾
				m_hitPointWorld.setInterpolate3(_fromPos, _toPos, rayResult.m_hitFraction);
				//�q�b�g�����R���W�����擾
				hitObject = (Collision*)m_collisionObject->getUserPointer();
			}
			return rayResult.m_hitFraction;
		}
	private:
		btVector3	_fromPos;				//���C�̎n�_
		btVector3	_toPos;					//���C�̏I�_
		int _attribute;						//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�
		float _Fraction = FLT_MAX;			//��r�p�ɕێ�
	public:
		btVector3	m_hitNormalWorld;		//�Փ˓_�̖@��
		btVector3	m_hitPointWorld;		//���C���q�b�g�����ʒu
		Collision* hitObject = nullptr;		//�q�b�g�����I�u�W�F�N�g
	};

	//�R���W�������΂��čŏ��Ƀq�b�g�����R���W������Ԃ��R�[���o�b�N
	struct ClosestConvexResultCallback : public btCollisionWorld::ConvexResultCallback
	{
	public:
		ClosestConvexResultCallback(const Vector3& start, const int& attr, const btCollisionObject* me)
		{
			_startPos = start;
			_attribute = attr;
			_me = me;
		}
		//�Փ˂����Ƃ��ɌĂ΂��֐��B
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			//���g�ƏՓ˂������H &&
			//����������Ȃ�����
			if (convexResult.m_hitCollisionObject == _me ||
				(_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0)
			{
				//�q�b�g���Ȃ������B
				return 0.0f;
			}

			//�q�b�g�����ʒu
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//�Փ˓_�̋��������߂�
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, _startPos);
			float distTmp = vDist.Length();
			//������r
			if (_dist > distTmp) {
				m_closestHitFraction = convexResult.m_hitFraction;
				//�X�V
				hitPos = hitPosTmp;
				//�@���X�V
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				_dist = distTmp;
				hitObject = (Collision*)convexResult.m_hitCollisionObject->getUserPointer();
			}
			return 0.0f;
		}
	private:
		int _attribute;										//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�B
		float _dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
		Vector3 _startPos = Vector3::zero;					//���C�̎n�_�B
		const btCollisionObject* _me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
	public:
		Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);		//�Փ˓_�B
		Vector3 hitNormal = Vector3::zero;					//�Փ˓_�̖@���B
		Collision* hitObject = nullptr;						//�q�b�g�����I�u�W�F�N�g
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
		Vector3 vDist = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		vector<unsigned int> Through_ID;		// ��������R���W���������B
		int	_attribute = 0;					//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�
		bool isSlip = false;	// ���邩�B
		float SlipAngle = 90.0f;			// ������Ƃ̊p�x�����̊p�x�ȏ�Ȃ犊��(�x)�B

		//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me) {
				//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
				return 0.0f;
			}

			if ((_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0) {
				// �Փ˂���肽���R���W�����ł͂Ȃ������B
				return 0.0f;
			}

			//�Փ˓_�̖@�������������Ă���B
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
			//// ���[���h�s��擾�B
			D3DXMATRIX worldMat;
			worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ���[���h���W�ɕϊ��B
			//D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);

			//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f))));

			if (((angle < D3DXToRadian(54.0f))		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
				|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::GROUND) //�������̓R���W�����������n�ʂƎw�肳��Ă���B
				|| convexResult.m_hitCollisionObject->getUserIndex() == BIT(7)
				) {

				//�Փ˂��Ă���B
				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitPosTmp), &hitPosTmp, &worldMat);	// ���[���h���W�ɕϊ��B			

				//��_�Ƃ̋����𒲂ׂ�B
				Vector3 vDistTmp;
				vDistTmp.Subtract(Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z), startPos);
				//vDistTmp.y = 0.0f;
				float distTmp = vDistTmp.Length();
				if (dist > distTmp) {
					//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
					hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z);
					dist = distTmp;
					vDist = vDistTmp;
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
					if (angle >= D3DXToRadian(SlipAngle)) {
						// �w��p�x�ȏ�Ȃ̂Ŋ��点��B
						isSlip = true;
					}
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
		Vector3 vDist = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3 hitNormal = Vector3::zero;	//�Փ˓_�̖@���B
		btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
		int	_attribute = 0;					//�w�肵���R���W���������Ƃ̂ݓ����蔻����Ƃ�

		//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me) {
				//�����ɏՓ˂����B
				return 0.0f;
			}

			if ((_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0) {
				// �Փ˂���肽���R���W�����ł͂Ȃ������B
				return 0.0f;
			}

			//�Փ˓_�̖@�������������Ă���B
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());		
			//// ���[���h�s��擾�B
			D3DXMATRIX worldMat;
			worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ���[���h���W�ɕϊ��B
			//D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);
			//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp,&D3DXVECTOR3(0.0f,1.0f,0.0f))));
			if (((angle >= D3DXToRadian(54.0f))		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
				|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::GROUND)
					|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::CHARACTER
					|| convexResult.m_hitCollisionObject->getUserIndex() == BIT(7)
				) {
				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitPosTmp), &hitPosTmp, &worldMat);	// ���[���h���W�ɕϊ��B			

				//��_�Ƃ̋����𒲂ׂ�B
				Vector3 vDistTmp;
				vDistTmp.Subtract(Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z), startPos);
				float distTmp = Vector3(vDistTmp.x,0.0f, vDistTmp.z).Length();
				if (distTmp < dist) {
					//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
					hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z) ;
					dist = distTmp;
					vDist = vDistTmp;
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
				}
			}

			return 0.0f;
		}
	};
}