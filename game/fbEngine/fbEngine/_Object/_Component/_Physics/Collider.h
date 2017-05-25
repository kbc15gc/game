#pragma once
#include "_Object\_Component\Component.h"

class ModelObject;

//�����蔻��̌`��
class Collider :public Component
{
public:
	Collider(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		this->_CollisionModel = nullptr;
	};
	Collider(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{
		this->_CollisionModel = nullptr;
	};
	~Collider();

	// �R���C�_�[�̌`������o�����邽�߂̃��f���𐶐�����֐��B
	// �����F	�R���C�_�[���A�^�b�`����I�u�W�F�N�g(�����I�ɂ��̃I�u�W�F�N�g�Ɛe�q�֌W���`������,null���Ɛe�q�֌W�������Ȃ��R���C�_�[�ƂȂ�)�B
	//			�R���W�����I�u�W�F�N�g�B
	//			���f���̒��S�_�ƃR���W�����̒��S�_�̍����B
	void CreateViewModel(GameObject* Parent,btCollisionObject* CollisionObject, const Vector3& Offset);
	// �R���C�_�[�̌`������o�����邽�߂̃��f���𐶐�����֐��B
	// �����F	�R���C�_�[���A�^�b�`����I�u�W�F�N�g(�����I�ɂ��̃I�u�W�F�N�g�Ɛe�q�֌W���`������,null���Ɛe�q�֌W�������Ȃ��R���C�_�[�ƂȂ�)�B
	//			Transform���B
	//			���f���̒��S�_�ƃR���W�����̒��S�_�̍����B
	void CreateViewModel(GameObject* Parent, const btTransform& Transform, const Vector3& Offset);

private:
	// �`��ɉ��������f���f�[�^�����[�h�B
	// ���p����Ŏ����B
	virtual void ColliderModelLoad() {}/* = 0*/;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	//�����蔻������o������3D�I�u�W�F�N�g�B
	ModelObject* _CollisionModel = nullptr;
};