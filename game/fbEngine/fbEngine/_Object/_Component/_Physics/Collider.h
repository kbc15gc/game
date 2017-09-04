#pragma once
#include "_Object\_Component\Component.h"

class ModelObject;
class Collision;

//�����蔻��̌`��
class Collider :public Component
{
public:
	enum class ShapeType { Box = 0, Sphere, Capsule, CapsuleX, CapsuleZ, Mesh };
#ifdef _DEBUG
	static const wchar_t* ShapeName[];
#endif
public:
	Collider(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		this->_CollisionModel = nullptr;
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	Collider(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{
#ifdef _DEBUG
		this->_CollisionModel = nullptr;
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~Collider();

#ifdef _DEBUG
	void Debug()override;
#endif
	// �R���C�_�[�̌`������o�����邽�߂̃��f���𐶐�����֐��B
	// �����F	�R���W������Transform���B
	void CreateViewModel(const btTransform& collisionTr);
	// �R���W�����`��p���f����Transform���X�V�B
	void UpdateTransform(const btTransform& collisionTr);

#ifdef _DEBUG
	// �`�撆���B
	bool GetIsRender();
	// �`��I���B
	void RenderEnable();
	// �`��I�t�B
	void RenderDisable();
#endif

	inline ShapeType GetType()const {
		return _Type;
	}
	inline const Vector3& GetHalfSize()const {
		return _halfSize;
	}
	inline void SetCollision(Collision* collision) {
		_collision = collision;
	}
private:
	// �`��ɉ��������f���f�[�^�����[�h�B
	// ���p����Ŏ����B
	virtual void ColliderModelLoad() = 0;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	ShapeType _Type;
	Vector3 _halfSize;
	Collision* _collision = nullptr;	// ���̃R���C�_�[��ێ�����R���W�����B
#ifdef _DEBUG
protected:
	//�����蔻������o������3D�I�u�W�F�N�g�B
	ModelObject* _CollisionModel = nullptr;
	Vector3 _CollisionModelOffset = Vector3::zero;	// �R���W�������o���p���f���̒��S�_�ƃR���W�����̒��S�_�̍���(�p����ɂ���ĕύX)�B
private:
	unique_ptr<Transform> _CollisionTr;	// �R���W������Transform���B
#endif
};