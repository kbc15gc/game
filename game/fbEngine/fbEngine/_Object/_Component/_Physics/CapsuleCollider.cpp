/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */
#include"fbstdafx.h"
#include "CapsuleCollider.h"


 /*!
 * @brief	�R���X�g���N�^�B
 */
CCapsuleCollider::CCapsuleCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	shape(nullptr)
{
	_Type = ShapeType::Capsule;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
* @brief	�f�X�g���N�^�B
*/
CCapsuleCollider::~CCapsuleCollider()
{
	delete shape;
}