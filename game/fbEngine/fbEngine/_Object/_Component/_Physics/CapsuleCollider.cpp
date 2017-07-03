/*!
 * @brief	カプセルコライダー。
 */
#include"fbstdafx.h"
#include "CapsuleCollider.h"


 /*!
 * @brief	コンストラクタ。
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
* @brief	デストラクタ。
*/
CCapsuleCollider::~CCapsuleCollider()
{
	delete shape;
}