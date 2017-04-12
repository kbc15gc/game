#include "BoxCollider.h"

/*!
 * @brief	�R���X�g���N�^�B
 */
BoxCollider::BoxCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	_Shape(NULL)
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
BoxCollider::~BoxCollider()
{
	SAFE_DELETE(_Shape);
}
/*!
 * @brief	�{�b�N�X�R���C�_�[���쐬�B
 */
void BoxCollider::Create( const Vector3& size )
{
	_Shape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
}