#include"fbstdafx.h"
#include "Collision.h"
#include "Collider.h"

#ifdef _DEBUG
const wchar_t* Collision::TypeName[] = {
	L"Rigidbody",
	L"GhostObject"
};
#endif

Collision::~Collision()
{
	_Shape = nullptr;

	// �V�F�A�[�h�|�C���^�Ȃ̂ŁA���̃R���|�[�l���g���폜���ꂽ����R���W�����I�u�W�F�N�g���Q�Ƃ���邱�Ƃ��l������B
	_CollisionObject->setUserPointer(nullptr);
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

void Collision::OnDestroy() {
	// ��RemoveWorld�֐��͓����ŉ��z�֐����Ăяo�����߁A�f�X�g���N�^�ł͐����������ƂȂ�Ȃ��B
	if (_CollisionObject)
	{
		RemoveWorld();
	}
}

void Collision::Create(btCollisionObject * collision, Collider * shape, const int & id, Vector3 offset,bool isAddWorld)
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

	if (isAddWorld) {
		AddWorld(); 
	}

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
	//�Q�[���I�u�W�F�N�g�̃g�����X�t�H�[�����𕨗��G���W���ɑ���B
	if (_CollisionObject.get() && _physicsType == PhysicsType::Kinematick) {
		//�R���W�����̃g�����X�t�H�[���̎Q�Ƃ��擾
		btTransform& trans = _CollisionObject->getWorldTransform();
		//�ړ���ݒ�
		trans.setOrigin(btVector3(GetOffsetPos().x, GetOffsetPos().y, GetOffsetPos().z));
		//��]��ݒ�
		trans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

		// �R���W�����`��p���f����Transform���X�V�B
		_Shape->UpdateTransform(trans);
	}
}

// ���[���h�ɓo�^�B
void Collision::AddWorld() {
	if (!_isAddWorld) {
		_UpdateCollisionTrans();	// �R���W�����o�^����AABB�{�b�N�X����������邽�߁A������Transform�����X�V����B
		_AddWorldSubClass();
		_isAddWorld = true;
	}
}
// ���[���h����폜�B
void Collision::RemoveWorld() {
	if (_isAddWorld) {
		_RemoveWorldSubClass();
		_isAddWorld = false;
	}
}
