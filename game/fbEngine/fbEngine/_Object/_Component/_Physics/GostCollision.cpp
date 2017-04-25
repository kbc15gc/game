#include "GostCollision.h"

GostCollision::~GostCollision()
{
	if (_CollisionObject)
	{
		//�������[���h����폜
		PhysicsWorld::Instance()->RemoveCollision(this);
	}
}

void GostCollision::Awake()
{
	Collision::Awake();
}

void GostCollision::Create(Collider * shape, int id)
{
	//�R���W��������
	Collision::Create(new btGhostObject, shape, id);
	//���|�C���^�擾���ăL���X�g
	_GostObject = btGhostObject::upcast(this->_CollisionObject.get());
	//���ݒ肳��Ă���t���O���擾
	int flags = _GostObject->getCollisionFlags();
	//���̃R���W�����ƏՓ˂��Ȃ��悤�Ƀt���O��ݒ�B
	_GostObject->setCollisionFlags(flags | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
	//�������[���h�ɓo�^
	PhysicsWorld::Instance()->AddCollision(this,_FilterGroup,_FilterMask);
}

void GostCollision::Update()
{
	Collision::Update();
}

btAlignedObjectArray<btCollisionObject*> GostCollision::GetPairCollisions()
{
	//���g�Əd�Ȃ��Ă���R���W�����擾
	return _GostObject->getOverlappingPairs();
}
