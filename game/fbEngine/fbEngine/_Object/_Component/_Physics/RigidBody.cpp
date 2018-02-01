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
		INSTANCE(PhysicsWorld)->RemoveRigidBody(this);
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
		//�V���~���[�g��̌��ʂ𑗂�
		btTransform trans = _CollisionObject->getWorldTransform();

		Vector3 pos = Vector3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
		btQuaternion rot = trans.getRotation();
		//�N�H�[�^�j�I�����e���̉�]�ʂɕϊ�
		transform->SetRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));

		//�|�W�V�����͂��炳��Ă���̂Ŗ߂��B
		pos = pos - (transform->GetRight() * _Offset.x);
		pos = pos - (transform->GetUp() * _Offset.y);
		pos = pos - (transform->GetForward() * _Offset.z);
		transform->SetPosition(pos);
		//transform->SetLocalPosition(Vector3(pos.x() - _Offset.x, pos.y() - _Offset.y, pos.z() - _Offset.z));

		_UpdateOffsetPos();
#ifdef _DEBUG
		_Shape->UpdateTransform();
#endif
	}
}
void RigidBody::OnEnable()
{
	_AddWorldSubClass();
}
void RigidBody::OnDisable()
{
	_RemoveWorldSubClass();
}
void RigidBody::Create(RigidBodyInfo& rbInfo, bool isAddWorld)
{
	//�O��̓��e���
	Release();
	_Offset = rbInfo.offset;
	//StartTrans.setOrigin(btVector3(transform->GetPosition().x + _Offset.x, transform->GetPosition().y + _Offset.y, transform->GetPosition().z + _Offset.z));
	// �I�t�Z�b�g�����ʒu�͉�]���l������B
	_UpdateOffsetPos();

	//��]�ƈړ��ݒ�
	btTransform StartTrans;
	StartTrans.setIdentity();
	//�|�W�V����
	StartTrans.setOrigin(btVector3(_OffsetPos.x, _OffsetPos.y, _OffsetPos.z));
	//��]
	StartTrans.setRotation(btQuaternion(rbInfo.rotation.x, rbInfo.rotation.y, rbInfo.rotation.z, rbInfo.rotation.w));
	myMotionState = new btDefaultMotionState(StartTrans);
	//���̂��쐬�B
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, myMotionState, rbInfo.coll->GetBody(), btVector3(rbInfo.inertia.x, rbInfo.inertia.y, rbInfo.inertia.z));
	Collision::Create(new btRigidBody(btRbInfo), rbInfo.coll, rbInfo.id, rbInfo.offset, isAddWorld);

	_MyObjectType = CollisionObjectType::Rigid;
	_physicsType = rbInfo.physicsType;
}

void RigidBody::Create(float mass, Collider* coll, int id, Vector3 inertia, Vector3 off,PhysicsType physicsType, bool isAddWorld)
{
	//�O��̓��e���
	Release();
	_Offset = off;

	// �I�t�Z�b�g�����ʒu�͉�]���l������B
	_UpdateOffsetPos();

	//��]�ƈړ��ݒ�
	btTransform StartTrans;
	StartTrans.setIdentity();
	//�|�W�V����
	StartTrans.setOrigin(btVector3(_OffsetPos.x, _OffsetPos.y, _OffsetPos.z));
	StartTrans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
	myMotionState = new btDefaultMotionState(StartTrans);
	//���̂��쐬�B
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(mass, myMotionState, coll->GetBody(), btVector3(inertia.x, inertia.y, inertia.z));
	Collision::Create(new btRigidBody(btRbInfo), coll, id, off,isAddWorld);

	_MyObjectType = CollisionObjectType::Rigid;
	_physicsType = physicsType;
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

void RigidBody::NonSleep()
{
	static_cast<btRigidBody*>(_CollisionObject.get())->setSleepingThresholds(0, 0);
}

void RigidBody::SetFilterMask_after(int mask)
{
	//��x���[���h��������H
	INSTANCE(PhysicsWorld)->RemoveRigidBody(this);
	//�}�X�N��ύX���čēx�ǉ��B
	_FilterMask = mask;
	INSTANCE(PhysicsWorld)->AddRigidBody(this, _FilterGroup, _FilterMask);
}

void RigidBody::_AddWorldSubClass() {
	INSTANCE(PhysicsWorld)->AddRigidBody(this);
}

void RigidBody::_RemoveWorldSubClass() {
	INSTANCE(PhysicsWorld)->RemoveRigidBody(this);
}
