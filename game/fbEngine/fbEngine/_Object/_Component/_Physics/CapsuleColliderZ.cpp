/*!
* @brief	カプセルコライダーZ。
*/
#include"fbstdafx.h"
#include "CapsuleColliderZ.h"


/*!
* @brief	コンストラクタ。
*/
CCapsuleColliderZ::CCapsuleColliderZ(GameObject* g, Transform* t) :
	Collider(g, t),
	shape(nullptr)
{
	_Type = ShapeType::CapsuleZ;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
* @brief	デストラクタ。
*/
CCapsuleColliderZ::~CCapsuleColliderZ()
{
	delete shape;
}

void CCapsuleColliderZ::Create(float radius, float height)
{
	if (radius < 0.0f || height < 0.0f) {
		// コライダーサイズに0より小さい値が設定されてるよ。
		abort();
	}
	shape = new btCapsuleShapeZ(radius, height);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(),work.z() + work.x());
}

void CCapsuleColliderZ::Resize(float radius, float height) {
	shape->setImplicitShapeDimensions(btVector3(radius, radius, height * 0.5f));
	_halfSize = Vector3(radius, radius, height * 0.5f + radius);
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
}
