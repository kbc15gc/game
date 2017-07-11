#include"fbstdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody(GameObject* g, Transform* t) :
	Collision(g,t,typeid(this).name()),
	myMotionState(NULL)
{
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}

RigidBody::~RigidBody()
{
	Release();
}
void RigidBody::Awake()
{
	Collision::Awake();
}
void RigidBody::Release()
{
	if (_CollisionObject)
	{
		SAFE_DELETE(myMotionState);
	}
}

void RigidBody::Update()
{
	Collision::Update();
}

void RigidBody::LateUpdate()
{
	if (_physicsType == PhysicsType::Dynamic)
	{
		//シュミレート後の結果を送る
		btTransform trans = _CollisionObject->getWorldTransform();

		btVector3 pos = trans.getOrigin();
		btQuaternion rot = trans.getRotation();
		//ポジションはずらされているので戻す。
		transform->SetLocalPosition(Vector3(pos.x() - _Offset.x, pos.y() - _Offset.y, pos.z() - _Offset.z));
		//クォータニオンを各軸の回転量に変換
		transform->SetRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
	}
}
void RigidBody::Create(RigidBodyInfo & rbInfo, bool isAddWorld)
{
	//前回の内容解放
	Release();
	_Offset = rbInfo.offset;
	//回転と移動設定
	btTransform StartTrans;
	StartTrans.setIdentity();
	//ポジション
	StartTrans.setOrigin(btVector3(transform->GetPosition().x + _Offset.x, transform->GetPosition().y + _Offset.y, transform->GetPosition().z + _Offset.z));
	//回転
	StartTrans.setRotation(btQuaternion(rbInfo.rotation.x, rbInfo.rotation.y, rbInfo.rotation.z, rbInfo.rotation.w));
	myMotionState = new btDefaultMotionState(StartTrans);
	//剛体を作成。
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, myMotionState, rbInfo.coll->GetBody(), btVector3(rbInfo.inertia.x, rbInfo.inertia.y, rbInfo.inertia.z));
	Collision::Create(new btRigidBody(btRbInfo), rbInfo.coll, rbInfo.id, rbInfo.offset, isAddWorld);

	_MyObjectType = CollisionObjectType::Rigid;
	_physicsType = rbInfo.physicsType;
}

void RigidBody::Create(float mass, Collider* coll, int id, Vector3 inertia, Vector3 off,PhysicsType physicsType, bool isAddWorld)
{
	//前回の内容解放
	Release();
	_Offset = off;
	//回転と移動設定
	btTransform StartTrans;
	StartTrans.setIdentity();
	StartTrans.setOrigin(btVector3(transform->GetPosition().x + _Offset.x, transform->GetPosition().y + _Offset.y, transform->GetPosition().z + _Offset.z));
	StartTrans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
	myMotionState = new btDefaultMotionState(StartTrans);
	//剛体を作成。
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(mass, myMotionState, coll->GetBody(), btVector3(inertia.x, inertia.y, inertia.z));
	Collision::Create(new btRigidBody(btRbInfo), coll, id, off,isAddWorld);

	_MyObjectType = CollisionObjectType::Rigid;
	_physicsType = physicsType;
}

void RigidBody::SetGravity(Vector3 set)
{
	//アップキャスト
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(btVector3(set.x, set.y, set.z));
}

void RigidBody::SetGravity(btVector3& set)
{
	//アップキャスト
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(set);
}

void RigidBody::SetGravity(float x, float y, float z)
{
	//アップキャスト
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(btVector3(x, y, z));
}

void RigidBody::NonSleep()
{
	static_cast<btRigidBody*>(_CollisionObject.get())->setSleepingThresholds(0, 0);
}

void RigidBody::_AddWorldSubClass() {
	INSTANCE(PhysicsWorld)->AddRigidBody(this);
}

void RigidBody::_RemoveWorldSubClass() {
	INSTANCE(PhysicsWorld)->RemoveRigidBody(this);
}
