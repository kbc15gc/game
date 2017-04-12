/*!
 * @brief	���̃R���C�_�[�B
 */

#pragma once

#include "Collider.h"

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
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btSphereShape*		shape;
};