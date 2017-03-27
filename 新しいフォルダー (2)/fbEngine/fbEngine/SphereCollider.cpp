/*!
 * @brief	���̃R���C�_�[�B
 */

#include "stdafx.h"
#include "SphereCollider.h"

/*!
 * @brief	�R���X�g���N�^�B
 */
SphereCollider::SphereCollider(GameObject* g, Transform* t) :
	Collider(g,t, typeid(this).name()),
	shape(NULL)
{

}
/*!
 * @brief	�f�X�g���N�^�B
 */
SphereCollider::~SphereCollider()
{
	delete shape;
}
/*!
 * @brief	���̃R���C�_�[���쐬�B
 */
void SphereCollider::Create( const float radius )
{
	shape = new btSphereShape(radius);
}
