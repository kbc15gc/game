#pragma once
#include "PhysicsCallback.h"

class Collision;
class RigidBody;
class Rigid;

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Start();
	void Update();
	void Render();
	/*!
	* @brief	�_�C�i�~�b�N���[���h���擾�B
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return dynamicWorld.get();
	}
	void AddRigidBody(RigidBody* rb);
	void RemoveRigidBody(RigidBody* rb);
	void AddRigid(Rigid* rb);
	void RemoveRigid(Rigid* rb);
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
	void AddCollision(Collision* coll);
	void RemoveCollision(Collision* coll);
	const Collision* FindOverlappedDamageCollision(btCollisionObject * colliObject,const int& id) const;
	const SweepResultGround FindOverlappedStage(btCollisionObject * colliObject,const Vector3& start,const Vector3& end) const;
	const SweepResultWall FindOverlappedWall(btCollisionObject * colliObject, const Vector3& start, const Vector3& end) const;

	static PhysicsWorld* Instance();
private:
	std::unique_ptr<btDefaultCollisionConfiguration>		collisionConfig;
	std::unique_ptr<btCollisionDispatcher>					collisionDispatcher;	//!<�Փˉ��������B
	std::unique_ptr<btBroadphaseInterface>					overlappingPairCache;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	std::unique_ptr<btSequentialImpulseConstraintSolver>	constraintSolver;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	std::unique_ptr<btDiscreteDynamicsWorld>				dynamicWorld;			//!<���[���h�B

	static PhysicsWorld* _Instance;
};