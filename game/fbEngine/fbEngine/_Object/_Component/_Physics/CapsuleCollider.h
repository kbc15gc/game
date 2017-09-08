#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//�J�v�Z����̃R���C�_�[
class CCapsuleCollider : public Collider
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	CCapsuleCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~CCapsuleCollider();
	/*!
	* @brief	�쐬�B
	*/
	void Create(float radius, float height);

	// �R���W�����̑傫�����Đݒ肷��֐��B
	// �����F	���a�B
	//			�����B
	void Resize(float radius, float height);

private:
#ifdef _DEBUG
	// �`��ɉ��������f���f�[�^�����[�h�B
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
	btCapsuleShape*		shape;
};