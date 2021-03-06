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
	//権利破棄
	dynamicWorld.release();
	constraintSolver.release();
	overlappingPairCache.release();
	collisionDispatcher.release();
	collisionConfig.release();
}

void PhysicsWorld::Start()
{
	//物理エンジンを初期化。
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
	//物理ワールドの重力設定
	dynamicWorld->setGravity(btVector3(0.0f, -9.8f*100, 0.0f));
	//ゴーストコリジョンがペアを見つけるときに使うコールバック設定。
	//これを設定しないとゴーストオブジェクトは重なったペアを見つけることはできない。
	dynamicWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback);
	dynamicWorld->getPairCache()->setInternalGhostPairCallback(new fbPhysicsCallback::MyGhostPairCallback);
}
void PhysicsWorld::Update()
{
	//シュミレーション
	dynamicWorld->stepSimulation((btScalar)Time::DeltaTime());
	dynamicWorld->updateAabbs();
}

void PhysicsWorld::AddRigidBody(RigidBody * rb, short group, short mask)
{
	dynamicWorld->addRigidBody((btRigidBody*)rb->GetCollisionObj(),group,mask);
}

void PhysicsWorld::RemoveRigidBody(RigidBody* rb)
{
	dynamicWorld->removeRigidBody((btRigidBody*)rb->GetCollisionObj());
}

void PhysicsWorld::AddCollision(Collision* coll, short group, short mask)
{
	dynamicWorld->addCollisionObject(coll->GetCollisionObj(),group,mask);
}

void PhysicsWorld::RemoveCollision(Collision * coll)
{
	dynamicWorld->removeCollisionObject(coll->GetCollisionObj());
}

fbPhysicsCallback::ClosestRayResultCallback PhysicsWorld::ClosestRayTest(const Vector3& f, const Vector3& t, int attr)
{
	//始点と終点を設定
	btVector3 from(f.x, f.y, f.z), to(t.x, t.y, t.z);
	//最も近かったを返すコールバック作成
	fbPhysicsCallback::ClosestRayResultCallback callback(from,to,attr);
	//レイを飛ばす
	dynamicWorld->rayTest(from, to, callback);
	return callback;
}

fbPhysicsCallback::ClosestConvexResultCallback PhysicsWorld::ClosestRayShape(Collider * shape, const Vector3 & from, const Vector3 & to,int attr)
{
	fbPhysicsCallback::ClosestConvexResultCallback callback(from, attr, nullptr);

	btTransform start, end;
	//初期化
	start.setIdentity();
	end.setIdentity();
	//ポジション設定
	start.setOrigin(btVector3(from.x, from.y, from.z));
	end.setOrigin(btVector3(to.x, to.y, to.z));

	dynamicWorld->convexSweepTest((const btConvexShape*)shape->GetBody(), start, end, callback);
	return callback;
}

bool PhysicsWorld::ContactPairTest(Collision* coll1, Collision* coll2, int attr)const {
	fbPhysicsCallback::ClosestContactResultCallback callback;
	callback.me = coll1;
	callback.attribute = attr;
	dynamicWorld->contactPairTest(coll1->GetCollisionObj(), coll2->GetCollisionObj(), callback);
	if (callback.hitObject) {
		return true;
	}
	else {
		return false;
	}
}

const Collision * PhysicsWorld::ClosestContactTest(Collision * coll, int attr) const
{
	fbPhysicsCallback::ClosestContactResultCallback callback;
	callback.me = coll;
	callback.attribute = attr;
	dynamicWorld->contactTest(coll->GetCollisionObj(), callback);

	return callback.hitObject;
}

const vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>>* PhysicsWorld::AllHitsContactTest(Collision * coll, vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>>* HitInfoArray, fbPhysicsCallback::AllHitsContactResultCallback* callback, int attr) const
{
	HitInfoArray->clear();
	callback->me = coll->gameObject;
	callback->attribute = attr;
	callback->SetHitInfoArray(HitInfoArray);
	dynamicWorld->contactTest(coll->GetCollisionObj(), *callback);

	return HitInfoArray;
}

const Collision * PhysicsWorld::ClosestConvexSweepTest(Collision * coll, const Vector3 & s, const Vector3 & e, int attr) const
{
	fbPhysicsCallback::ClosestConvexResultCallback callback(s, attr, coll->GetCollisionObj());

	btTransform start, end;
	//初期化
	start.setIdentity();
	end.setIdentity();
	//ポジション設定
	start.setOrigin(btVector3(s.x, s.y, s.z));
	end.setOrigin(btVector3(e.x, e.y, e.z));

	dynamicWorld->convexSweepTest((const btConvexShape*)coll->GetCollisionObj()->getCollisionShape(), start, end, callback);
	return callback.hitObject;
}

const fbPhysicsCallback::SweepResultGround PhysicsWorld::FindOverlappedStage(btCollisionObject * colliObject,const Vector3& s,const Vector3& e) const
{
	fbPhysicsCallback::SweepResultGround callback;
	btTransform start, end;
	//初期化
	start.setIdentity();
	end.setIdentity();
	//ポジション設定
	start.setOrigin(btVector3(s.x, s.y, s.z));
	end.setOrigin(btVector3(e.x, e.y, e.z));

	callback.me = static_cast<Collision*>(colliObject->getUserPointer())->gameObject;
	callback.startPos.Set(s);
	dynamicWorld->convexSweepTest((const btConvexShape*)colliObject->getCollisionShape(), start, end, callback);
	return callback;
}