#include "GostCollision.h"
#include "Collider.h"

GostCollision::~GostCollision()
{
	if (_CollisionObject)
	{
		//�o�^����Ă���̂ō폜
		PhysicsWorld::Instance()->RemoveCollision(this);
	}
}

void GostCollision::Create(Collider * shape, int id)
{
	//�R���W��������
	Collision::Create(new btGhostObject, shape, id);
	//��Ɨp�ϐ�
	//�R���W�����I�u�W�F�N�g
	btCollisionObject* coll = this->_CollisionObject.get();
	//�t���O
	int flags = coll->getCollisionFlags();
	//�R���W�����ɏՓ˂��Ă������I���������Ȃ�(������Ȃ��B)
	//�����Ńg���K�[�w�肵�Ȃ��ƕ����I�Ɋ����Ă���A�S�[�X�g�Ȃ̂ɁB
	coll->setCollisionFlags(flags | btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE);
	//���g��o�^
	PhysicsWorld::Instance()->AddCollision(this);
}

void GostCollision::Update()
{
	Collision::Update();
}