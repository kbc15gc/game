#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//���^�̃R���C�_�[
class BoxCollider : public Collider
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	BoxCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~BoxCollider();
	/*!
	 * @brief	�{�b�N�X�R���C�_�[���쐬�B
	 */
	void Create( const Vector3& size );

private:
	// �`��ɉ��������f���f�[�^�����[�h�B
	void ColliderModelLoad()override;

public:
	btCollisionShape* GetBody() override
	{
		return _Shape;
	}
private:
	btBoxShape*		_Shape;
};