/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#include "CapsuleCollider.h"


 /*!
 * @brief	�R���X�g���N�^�B
 */
CCapsuleCollider::CCapsuleCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	shape(nullptr)
{
}
/*!
* @brief	�f�X�g���N�^�B
*/
CCapsuleCollider::~CCapsuleCollider()
{
	delete shape;
}