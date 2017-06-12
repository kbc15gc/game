#include"fbstdafx.h"
#include "Physics.h"

PhysicsWorld* PhysicsWorld::_Instance = nullptr;

PhysicsWorld::PhysicsWorld()
{
	collisionConfig = NULL;
	collisionDispatcher = NULL;
	overlappingPairCache = NULL;
	constraintSolver = NULL;
	dynamicWorld = NULL;
}


PhysicsWorld::~PhysicsWorld()
{
	//�����j��
	dynamicWorld.release();
	constraintSolver.release();
	overlappingPairCache.release();
	collisionDispatcher.release();
	collisionConfig.release();
}

void PhysicsWorld::Start()
{
	//�����G���W�����������B
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfig.reset(new btDefaultCollisionConfiguration());

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	collisionDispatcher.reset(new btCollisionDispatcher(collisionConfig.get()));

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache.reset(new btDbvtBroadphase());

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	constraintSolver.reset(new btSequentialImpulseConstraintSolver);

	dynamicWorld.reset(new btDiscreteDynamicsWorld(
		collisionDispatcher.get(),
		overlappingPairCache.get(),
		constraintSolver.get(),
		collisionConfig.get()
		));
	//�������[���h�̏d�͐ݒ�
	dynamicWorld->setGravity(btVector3(0.0f, -9.8f*100, 0.0f));
	//�S�[�X�g�R���W�������y�A��������Ƃ��Ɏg���R�[���o�b�N�ݒ�B
	//�����ݒ肵�Ȃ��ƃS�[�X�g�I�u�W�F�N�g�͏d�Ȃ����y�A�������邱�Ƃ͂ł��Ȃ��B
	dynamicWorld->getPairCache()->setInternalGhostPairCallback(new fbPhysicsCallback::MyGhostPairCallback);
}
void PhysicsWorld::Update()
{
	dynamicWorld->stepSimulation((btScalar)Time::DeltaTime());
	dynamicWorld->updateAabbs();
}

void PhysicsWorld::AddRigidBody(RigidBody * rb, short group, short mask)
{
	dynamicWorld->addRigidBody((btRigidBody*)rb->GetCollisonObj(),group,mask);
}

void PhysicsWorld::RemoveRigidBody(RigidBody* rb)
{
	dynamicWorld->removeRigidBody((btRigidBody*)rb->GetCollisonObj());
}

void PhysicsWorld::AddCollision(Collision* coll, short group, short mask)
{
	dynamicWorld->addCollisionObject(coll->GetCollisonObj(),group,mask);
}

void PhysicsWorld::RemoveCollision(Collision * coll)
{
	dynamicWorld->removeCollisionObject(coll->GetCollisonObj());
}

fbPhysicsCallback::ClosestRayResultCallback PhysicsWorld::ClosestRayTest(const Vector3& f, const Vector3& t, const int& attr)
{
	//�n�_�ƏI�_��ݒ�
	btVector3 from(f.x, f.y, f.z), to(t.x, t.y, t.z);
	//�ł��߂�������Ԃ��R�[���o�b�N�쐬
	fbPhysicsCallback::ClosestRayResultCallback callback(from,to,attr);
	//���C���΂�
	dynamicWorld->rayTest(from, to, callback);
	return callback;
}

fbPhysicsCallback::ClosestConvexResultCallback PhysicsWorld::ClosestRayShape(Collider * shape, const Vector3 & from, const Vector3 & to, const int & attr)
{
	fbPhysicsCallback::ClosestConvexResultCallback callback(from, attr, nullptr);

	btTransform start, end;
	//������
	start.setIdentity();
	end.setIdentity();
	//�|�W�V�����ݒ�
	start.setOrigin(btVector3(from.x, from.y, from.z));
	end.setOrigin(btVector3(to.x, to.y, to.z));

	dynamicWorld->convexSweepTest((const btConvexShape*)shape->GetBody(), start, end, callback);
	return callback;
}

const Collision * PhysicsWorld::ClosestContactTest(Collision * coll,const int& attr) const
{
	fbPhysicsCallback::ClosestContactResultCallback callback;
	callback.me = coll;
	callback.attribute = attr;
	dynamicWorld->contactTest(coll->GetCollisonObj(), callback);

	return callback.hitObject;
}

const vector<Collision*> PhysicsWorld::AllHitsContactTest(Collision * coll, const int & attr) const
{
	fbPhysicsCallback::AllHitsContactResultCallback callback;
	callback.me = coll;
	callback.attribute = attr;
	dynamicWorld->contactTest(coll->GetCollisonObj(), callback);

	// callback�̓��[�J���ϐ��̂��߁A�Q�ƂŔz���Ԃ��Ă��֐��𔲂���Ə�񂪃��X�g����B
	// ���Ƃ肠�����l�ŕԋp����B
	return callback.hitObjects;
}

const Collision * PhysicsWorld::ClosestConvexSweepTest(Collision * coll, const Vector3 & s, const Vector3 & e, const int & attr) const
{
	fbPhysicsCallback::ClosestConvexResultCallback callback(s, attr, coll->GetCollisonObj());

	btTransform start, end;
	//������
	start.setIdentity();
	end.setIdentity();
	//�|�W�V�����ݒ�
	start.setOrigin(btVector3(s.x, s.y, s.z));
	end.setOrigin(btVector3(e.x, e.y, e.z));

	dynamicWorld->convexSweepTest((const btConvexShape*)coll->GetCollisonObj()->getCollisionShape(), start, end, callback);
	return callback.hitObject;
}

const fbPhysicsCallback::SweepResultGround PhysicsWorld::FindOverlappedStage(btCollisionObject * colliObject,const Vector3& s,const Vector3& e) const
{
	fbPhysicsCallback::SweepResultGround callback;
	btTransform start, end;
	//������
	start.setIdentity();
	end.setIdentity();
	//�|�W�V�����ݒ�
	start.setOrigin(btVector3(s.x, s.y, s.z));
	end.setOrigin(btVector3(e.x, e.y, e.z));

	callback.me = colliObject;
	callback.startPos.Set(s);
	dynamicWorld->convexSweepTest((const btConvexShape*)colliObject->getCollisionShape(), start, end, callback);
	return callback;
}