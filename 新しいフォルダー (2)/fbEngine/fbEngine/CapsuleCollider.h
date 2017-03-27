/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#pragma once

#include "Collider.h"

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
	void Create(float radius, float height)
	{
		shape = new btCapsuleShape(radius, height);
	}
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btCapsuleShape*		shape;
};