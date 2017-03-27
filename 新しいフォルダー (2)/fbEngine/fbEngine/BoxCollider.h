#pragma once

#include "Collider.h"

/*!
 * @brief	�{�b�N�X�R���C�_�[�B
 */
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
	btCollisionShape* GetBody() override
	{
		return _Shape;
	}
private:
	btBoxShape*		_Shape;
};