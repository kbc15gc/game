#pragma once
#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

class ModelObject;
//�����蔻��̊��N���X
class Collision:public Component
{
public:
	Collision(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
	
	};
	Collision(GameObject* g, Transform* t,const char* classname) :
		Component(g, t, classname)
	{
		
	};
	virtual ~Collision();
	void Awake()override;
	virtual void Update();
	//�R���W��������
	//������ btCollisionObject* �o���b�g�t�B�W�b�N�X�̃R���W�����I�u�W�F�N�g
	//������ Collider* �����蔻��̌`��
	//��O���� const int �R���W�����ɐݒ肷��ID
	//��l���� Vector3& ��_����̈ړ���
	void Create(btCollisionObject* collision, Collider* shape, const int& id = static_cast<int>(fbCollisionAttributeE::ALL),Vector3 offset = Vector3::zero);
	btCollisionObject* GetCollisonObj() const
	{
		return _CollisionObject.get();
	}
	//�I�t�Z�b�g������̃|�W�V�����擾
	const Vector3 GetOffsetPos()
	{
		return transform->GetPosition() + _Offset;
	}

	//Create���O�ɐݒ肵�Ƃ��āB
	// �S���C���[�}�X�N�I�t�B
	// ���ׂĂ̏Փ˂𖳎��B
	inline void FilterMask_AllOff() {
		SetFilterMask(0x00000000);
	}

	// �S���C���[�}�X�N�I���B
	// ���ׂẴR���W�����Ɠ����蔻����s���B
	inline void FilterMask_AllOn() {
		SetFilterMask(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// �t�B���^�[�}�X�N�ɉ��Z�B
	inline void AddFilterMask(short bit) {
		SetFilterMask(_FilterMask | bit);
	}

	// �t�B���^�[�}�X�N���猸�Z�B
	inline void SubFilterMask(short bit) {
		// ���ׂĂ�bit�𔽓]���A�ړI�̃r�b�g�̂�0�A����1�ɂ���B
		bit = ~bit;
		SetFilterMask(_FilterMask & bit);
	}

	void SetFilter(short group, short mask);
	void SetFilterGroup(short group);
	void SetFilterMask(short mask);
	inline short GetFilterGroup()const {
		return _FilterGroup;
	}
	inline short GetFilterMask()const {
		return _FilterMask;
	}
	// ���̃R���W�����������ɐݒ肵��������ԋp�B
	inline int GetID()const {
		return _CollisionObject->getUserIndex();
	}
protected:
	//�R���W�����̈ʒu���]���X�V
	void _UpdateCollisionTrans();
protected:
	//�|�W�V��������̃I�t�Z�b�g��
	Vector3 _Offset;
	//�����̑�����O���[�v
	short _FilterGroup;
	//�����蔻����Ƃ�O���[�v
	short _FilterMask;
	//�R���W�����̌`��B
	Collider *_Shape;
	//�R���W�����I�u�W�F�N�g�B
	std::shared_ptr<btCollisionObject>	_CollisionObject;
};