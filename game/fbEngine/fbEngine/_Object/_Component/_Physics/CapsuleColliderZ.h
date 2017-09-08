#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//�J�v�Z����̃R���C�_�[Z
class CCapsuleColliderZ : public Collider
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	CCapsuleColliderZ(GameObject* g, Transform* t);
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~CCapsuleColliderZ();
	/*!
	* @brief	�쐬�B
	*/
	void Create(float radius, float height);

	// �R���W�����̑傫�����Đݒ肷��֐��B
	// �����F	���a�B
	//			�����B
	void Resize(float radius, float height);

private:
	// �`��ɉ��������f���f�[�^�����[�h�B
#ifdef _DEBUG
	void ColliderModelLoad()override {
		// �J�v�Z���͂܂��Ȃ��̂Ő������Ȃ��B
	}
#endif

public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btCapsuleShapeZ*		shape;
};