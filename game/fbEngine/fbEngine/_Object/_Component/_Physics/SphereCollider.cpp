/*!
 * @brief	球体コライダー。
 */
#include"fbstdafx.h"
#include "_Object\_Component\_Physics\SphereCollider.h"
#include "_Object\_GameObject\ModelOject.h"

/*!
 * @brief	コンストラクタ。
 */
SphereCollider::SphereCollider(GameObject* g, Transform* t) :
	Collider(g,t, typeid(this).name()),
	shape(NULL)
{
	_Type = ShapeType::Sphere;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
 * @brief	デストラクタ。
 */
SphereCollider::~SphereCollider()
{
	delete shape;
}
/*!
 * @brief	球体コライダーを作成。
 */
void SphereCollider::Create( const float radius )
{
	if (radius < 0.0f) {
		// サイズに0より小さい値が設定されている。
		abort();
	}
	shape = new btSphereShape(radius);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(), work.z());
}

void SphereCollider::Resize(float radius) {
	shape->setImplicitShapeDimensions(btVector3(radius, radius, radius));
	_halfSize = Vector3(radius, radius, radius);
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
}

void SphereCollider::ColliderModelLoad() {
#ifdef _DEBUG

	//当たり判定を視覚化したオブジェクト生成
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("SphereCollision.X");
#endif
}