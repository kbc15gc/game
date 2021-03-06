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

void CCapsuleCollider::Create(float radius, float height)
{
	if (radius < 0.0f || height < 0.0f) {
		// コライダーサイズに0より小さい値が設定されてるよ。
		abort();
	}
	shape = new btCapsuleShape(radius, height);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y() + work.x(), work.z());
}

void CCapsuleCollider::Resize(float radius, float height) {
	SAFE_DELETE(shape);
	shape = new btCapsuleShape(radius, height);
	_collision->GetCollisionObj()->setCollisionShape(shape);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y() + work.x(), work.z());
#ifdef _DEBUG
	RecreateViewModel();
#endif
}
