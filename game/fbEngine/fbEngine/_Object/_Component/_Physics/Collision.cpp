#include "Collision.h"

#ifdef _DEBUG
#include "_Object\_GameObject\ModelOject.h"
#endif // _DEBUG


Collision::~Collision()
{
	_Shape = nullptr;
	//�V�F�A�[�h�|�C���^�Ȃ̂�null�����邾����OK
	_CollisionObject = nullptr;
#ifdef _DEBUG
	if(_CollisionModel)
	INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);
#endif // _DEBUG
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

#ifdef _DEBUG
	//�O�ɐݒ肳��Ă����A�h���X���폜
	if (_CollisionModel)
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);

	//�`��擾
	int type = _Shape->GetBody()->getShapeType();
	//�`�ɑΉ��������f���ǂݍ���
	switch (type)
	{
	case 0:	//box
		//�����蔻������o�������I�u�W�F�N�g����
		_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
		_CollisionModel->LoadModel("Debug/BoxCollision.X");
		break;
	case 8:	//sphere
		//�����蔻������o�������I�u�W�F�N�g����
		_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
		_CollisionModel->LoadModel("Debug/SphereCollision.X");
		break;
	default:
		break;
	}
	//���f��������̂Ȃ�
	if (_CollisionModel)
	{
		_CollisionModel->GetSkinModel()->SetModelEffect(ModelEffectE::NONE);
		//�������ȐԂɐݒ�B
		_CollisionModel->GetSkinModel()->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
		//�q�ɐݒ�
		_CollisionModel->transform->SetParent(this->transform);
		//�����蔻��̑傫���𒲂ׂ�B
		btTransform t = _CollisionObject->getWorldTransform();
		btVector3 min, max;
		_Shape->GetBody()->getAabb(t, min, max);
		Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
		//�����蔻��̃T�C�Y���w��
		_CollisionModel->transform->SetLocalScale(size);
		//�����蔻������炷
		_CollisionModel->transform->SetLocalPosition(_Offset);
	}
#endif // _DEBUG
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
}