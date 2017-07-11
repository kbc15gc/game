#include"fbstdafx.h"
#include "GostCollision.h"

GostCollision::~GostCollision()
{
}

void GostCollision::Awake()
{
	Collision::Awake();
}

void GostCollision::Create(Collider * shape, int id, bool isAddWorld)
{
	//�R���W��������
	Collision::Create(new btGhostObject, shape, id,Vector3::zero,isAddWorld);
	//���|�C���^�擾���ăL���X�g
	_GostObject = btGhostObject::upcast(this->_CollisionObject.get());
	//���ݒ肳��Ă���t���O���擾
	int flags = _GostObject->getCollisionFlags();
	//���̃R���W�����ƏՓ˂��Ȃ��悤�Ƀt���O��ݒ�B
	_GostObject->setCollisionFlags(flags | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);

	_MyObjectType = CollisionObjectType::Ghost;
	_physicsType = PhysicsType::Kinematick;	// �S�[�X�g�͂Ƃ肠�����L�l�}�e�B�b�N���́B
}

void GostCollision::Update()
{
	Collision::Update();
}

const btAlignedObjectArray<btCollisionObject*>& GostCollision::GetPairCollisions()
{
	//���g�Əd�Ȃ��Ă���R���W�����擾
	return _GostObject->getOverlappingPairs();
}

void GostCollision::_AddWorldSubClass() {
	INSTANCE(PhysicsWorld)->AddCollision(this);
}

void GostCollision::_RemoveWorldSubClass() {
	INSTANCE(PhysicsWorld)->RemoveCollision(this);
}
