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

	void Update()override;
	// �R���C�_�[�̌`������o�����邽�߂̃��f���𐶐�����֐��B
	// �����F	�R���W������Transform���B
	void CreateViewModel(const btTransform& collisionTr);
	// �R���W�����`��p���f����Transform���X�V�B
	void UpdateTransform(const btTransform& collisionTr);

	// �`�撆���B
	bool GetIsRender();
	// �`��I���B
	void RenderEnable();
	// �`��I�t�B
	void RenderDisable();
private:
	// �`��ɉ��������f���f�[�^�����[�h�B
	// ���p����Ŏ����B
	virtual void ColliderModelLoad() = 0;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	//�����蔻������o������3D�I�u�W�F�N�g�B
	ModelObject* _CollisionModel = nullptr;
	Vector3 _CollisionModelOffset = Vector3::zero;	// �R���W�������o���p���f���̒��S�_�ƃR���W�����̒��S�_�̍���(�p����ɂ���ĕύX)�B
private:
	unique_ptr<Transform> _CollisionTr;	// �R���W������Transform���B
};