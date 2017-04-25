#pragma once
#include "_Object\_Component\Component.h"

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
	void Create(btCollisionObject* collision, Collider* shape, const int& id,Vector3 offset = Vector3::zero);
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
	void SetFilter(short group, short mask);
	void SetFilterGroup(short group);
	void SetFilterMask(short mask);
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
private:
	//�����蔻������o������3D�I�u�W�F�N�g�B
	ModelObject* _CollisionModel;
};