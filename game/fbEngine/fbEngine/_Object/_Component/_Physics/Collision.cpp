#include"fbstdafx.h"
#include "Collision.h"
#include "Collider.h"

Collision::~Collision()
{
	_Shape = nullptr;
	//�V�F�A�[�h�|�C���^�Ȃ̂�null�����邾����OK
	_CollisionObject = nullptr;
}
void Collision::Awake()
{
	_Offset = Vector3::zero;
	_Shape = nullptr;
	_CollisionObject = nullptr;
	_FilterGroup = (short)fbCollisionFilterE::A;
	_FilterMask = (short)fbCollisionFilterE::ALLFILTER;
}

void Collision::Update()
{
	//�g�����X�t�H�[���̍X�V
	_UpdateCollisionTrans();
}

void Collision::Create(btCollisionObject * collision, Collider * shape, const int & id, Vector3 offset)
{
	_Offset = offset;
	_Shape = shape;
	//�R���W�����I�u�W�F�N�g�ݒ�
	_CollisionObject.reset(collision);
	_CollisionObject->setCollisionShape(_Shape->GetBody());
	_CollisionObject->setUserPointer(this);	//�R���|�[�l���g�̃A�h���X�ݒ�B
	_CollisionObject->setUserIndex(id);		//�R���W����ID�ݒ�
	//�g�����X�t�H�[���X�V
	_UpdateCollisionTrans();

	// �Ƃ肠���������ŃR���C�_�[�̕`����I���ɂ���B
	_Shape->CreateViewModel(_CollisionObject->getWorldTransform());
}

void Collision::SetFilter(short group, short mask)
{
	_FilterGroup = group;
	_FilterMask = mask;
}

void Collision::SetFilterGroup(short group)
{
	_FilterGroup = group;
}

void Collision::SetFilterMask(short mask)
{
	_FilterMask = mask;
}

void Collision::_UpdateCollisionTrans()
{
	//�R���W�����̃g�����X�t�H�[���̎Q�Ƃ��擾
	btTransform& trans = _CollisionObject->getWorldTransform();
	//�ړ���ݒ�
	trans.setOrigin(btVector3(GetOffsetPos().x, GetOffsetPos().y, GetOffsetPos().z));
	//��]��ݒ�
	trans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
	
	// �R���W�����`��p���f����Transform���X�V�B
	_Shape->UpdateTransform(trans);
}