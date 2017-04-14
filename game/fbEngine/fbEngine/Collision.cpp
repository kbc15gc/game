#include "Collision.h"
#include "Collider.h"


Collision::~Collision()
{
	_Shape = nullptr;
	_CollisionObject = nullptr;
}
#ifdef _DEBUG
#include "GameObject.h"
#include "SkinModel.h"
#include "SkinModelData.h"
#endif // _DEBUG
void Collision::Awake()
{
#ifdef _DEBUG
	//�l�p�����f���`��
	SkinModel* model = gameObject->AddComponent<SkinModel>();
	SkinModelData* data = new SkinModelData();
	//���̂����`�ς�����
	data->CloneModelData(SkinModelManager::LoadModel("BoxCollision.X"));
	model->SetModelData(data);
	//�Ԃ��R���W����
	model->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
#endif // _DEBUG

	_Offset = Vector3::zero;
	_Shape = nullptr;
	_CollisionObject = nullptr;
}

void Collision::Update()
{
	//���t���[��������
	_IsHit = false;
	//�g�����X�t�H�[���̍X�V
	_UpdateCollisionTrans();
}

void Collision::Create(btCollisionObject * collision, Collider * shape, const int & id, Vector3 offset)
{
	_IsHit = false;
	_Offset = offset;
	_Shape = shape;
	//�R���W�����I�u�W�F�N�g�ݒ�
	_CollisionObject.reset(collision);
	_CollisionObject->setCollisionShape(_Shape->GetBody());
	_CollisionObject->setUserPointer(this);	//�|�C���^�ݒ�
	_CollisionObject->setUserIndex(id);		//�R���W����ID�ݒ�

	//�g�����X�t�H�[���X�V
	_UpdateCollisionTrans();
}

void Collision::_UpdateCollisionTrans()
{
	//�R���W�����̃g�����X�t�H�[���̎Q�Ƃ��擾
	btTransform& trans = _CollisionObject->getWorldTransform();
	//�ړ���ݒ�
	trans.setOrigin(btVector3(GetOffsetPos().x, GetOffsetPos().y, GetOffsetPos().z));
	//��]��ݒ�
	trans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
}