#pragma once
#include "_Object\_Component\_Physics\Collider.h"

//����̃R���C�_�[
class SphereCollider : public Collider
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	SphereCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~SphereCollider();
	/*!
	 * @brief	���̃R���C�_�[���쐬�B
	 */
	void Create( const float radius );

private:
//	 �`��ɉ��������f���f�[�^�����[�h�B
	void ColliderModelLoad()override;

public:
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btSphereShape*		shape;
};