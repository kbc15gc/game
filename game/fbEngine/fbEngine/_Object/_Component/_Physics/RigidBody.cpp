#include"fbstdafx.h"
#include "RigidBody.h"

RigidBody::RigidBody(GameObject* g, Transform* t) :
	Collision(g,t,typeid(this).name()),
	myMotionState(NULL)
{
	
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
	//�V���~���[�g��̌��ʂ𑗂�
	btTransform trans = _CollisionObject->getWorldTransform();
	
	btVector3 pos = trans.getOrigin();
	btQuaternion rot = trans.getRotation();
	//�|�W�V�����͂��炳��Ă���̂Ŗ߂��B
	transform->SetLocalPosition(Vector3(pos.x() - _Offset.x, pos.y() - _Offset.y, pos.z() - _Offset.z));
	//�N�H�[�^�j�I�����e���̉�]�ʂɕϊ�
	transform->SetRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
}

//void RigidBody::Create(RigidBodyInfo& rbInfo)
void RigidBody::Create(float mass, Collider* coll, int id, Vector3 inertia, Vector3 off, bool isAddWorld)
{
	//�O��̓��e���
	Release();
	//��]�ƈړ��ݒ�
	btTransform StartTrans;
	StartTrans.setIdentity();
	StartTrans.setOrigin(btVector3(transform->GetPosition().x + _Offset.x, transform->GetPosition().y + _Offset.y, transform->GetPosition().z + _Offset.z));
	StartTrans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
	myMotionState = new btDefaultMotionState(StartTrans);
	//���̂��쐬�B
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(mass, myMotionState, coll->GetBody(), btVector3(inertia.x, inertia.y, inertia.z));
	Collision::Create(new btRigidBody(btRbInfo), coll, id, off,isAddWorld);

	_MyObjectType = CollisionObjectType::Rigid;
}

void RigidBody::SetGravity(Vector3 set)
{
	//�A�b�v�L���X�g
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(btVector3(set.x, set.y, set.z));
}

void RigidBody::SetGravity(btVector3& set)
{
	//�A�b�v�L���X�g
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(set);
}

void RigidBody::SetGravity(float x, float y, float z)
{
	//�A�b�v�L���X�g
	btRigidBody* rigit = (btRigidBody*)_CollisionObject.get();
	rigit->setGravity(btVector3(x, y, z));
}

void RigidBody::_AddWorldSubClass() {
	INSTANCE(PhysicsWorld)->AddRigidBody(this);
}

void RigidBody::_RemoveWorldSubClass() {
	INSTANCE(PhysicsWorld)->RemoveRigidBody(this);
}
