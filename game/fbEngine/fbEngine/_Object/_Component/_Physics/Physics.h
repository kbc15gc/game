#pragma once
#include "_Include\PhysicsCallback.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Start();
	void Update();

	//���[���h�ɍ��̒ǉ�
	void AddRigidBody(RigidBody* rb, short group = (short)fbCollisionFilterE::A, short mask = (short)fbCollisionFilterE::ALLFILTER);
	//���[���h���獄�̍폜
	void RemoveRigidBody(RigidBody* rb);
	//���[���h�ɃR���W�����ǉ�
	void AddCollision(Collision* coll, short group = (short)fbCollisionFilterE::A, short mask = (short)fbCollisionFilterE::ALLFILTER);
	//���[���h����R���W�����폜
	void RemoveCollision(Collision* coll);

	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}

	//�w�肵���R���W�������m���Փ˂��Ă��邩���肷��
	//Collision*�@�R���W����
	//Collision*  �R���W����
	//int�@�w�肵���R���W�����̑����Ƃ̂ݓ�������Ƃ�
	bool ContactPairTest(Collision* coll1, Collision* coll2, int attr = static_cast<int>(fbCollisionAttributeE::ALL))const;
	//�q�b�g�������ōł��߂��R���W�������擾
	//Collision*�@�R���W����
	//int�@�w�肵���R���W�����̑����Ƃ̂ݓ�������Ƃ�
	const Collision* ClosestContactTest(Collision * coll,int attr = (int)fbCollisionAttributeE::ALL) const;
	//�q�b�g�����R���W������S�Ď擾
	//�����F�@�R���W����
	//		  �Փ˂����R���W�����̔z��B
	//		�@�w�肵���R���W�����̑����Ƃ̂ݓ�������Ƃ�
	// �߂�l�͑������Ɠ����B
	const vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>>* AllHitsContactTest(Collision * coll, vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>>* HitInfoArray, fbPhysicsCallback::AllHitsContactResultCallback* callback, int attr = (int)fbCollisionAttributeE::ALL) const;
	//���C���΂��ăq�b�g�������ōł��߂��������̂��擾
	//Vector3�@���C�̎n�_
	//Vector3�@���C�̏I�_
	//int�@�w�肵���R���W�����̑����Ƃ̂ݓ�������Ƃ�
	fbPhysicsCallback::ClosestRayResultCallback ClosestRayTest(const Vector3& from, const Vector3& to, int attr = (int)fbCollisionAttributeE::ALL);
	//�`�̂������΂�
	fbPhysicsCallback::ClosestConvexResultCallback ClosestRayShape(Collider* shape,const Vector3& from, const Vector3& to, int attr = (int)fbCollisionAttributeE::ALL);
	//�󂯎�����R���W�������n�_����I�_�܂Ŕ�΂��ăq�b�g�������ōł��߂��������̂��擾
	//Collision* ��΂��R���W����
	//Vector3�@�n�_
	//Vector3�@�I�_
	//int�@�w�肵���R���W�����̑����Ƃ̂ݓ�������Ƃ�
	const Collision* ClosestConvexSweepTest(Collision * coll, const Vector3& start, const Vector3& end,int attr = (int)fbCollisionAttributeE::ALL) const;
	//�n�ʗp�����蔻��
	const fbPhysicsCallback::SweepResultGround FindOverlappedStage(btCollisionObject * colliObject,const Vector3& start,const Vector3& end) const;

	//�_�C�i�~�b�N���[���h�擾
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld.get();
	}

	static PhysicsWorld* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new PhysicsWorld();
		}
		return _Instance;
	}
private:
	std::unique_ptr<btDefaultCollisionConfiguration>		collisionConfig;
	std::unique_ptr<btCollisionDispatcher>					collisionDispatcher;	//!<�Փˉ��������B
	std::unique_ptr<btBroadphaseInterface>					overlappingPairCache;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	std::unique_ptr<btSequentialImpulseConstraintSolver>	constraintSolver;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	std::unique_ptr<btDiscreteDynamicsWorld>				dynamicWorld;			//!<���[���h�B

	static PhysicsWorld* _Instance;
};