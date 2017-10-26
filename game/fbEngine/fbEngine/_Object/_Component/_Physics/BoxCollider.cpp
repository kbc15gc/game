#include"fbstdafx.h"
#include "BoxCollider.h"
#include "_Object\_GameObject\ModelOject.h"
#include "_Object\_Component\_Physics\Collision.h"

/*!
 * @brief	コンストラクタ。
 */
BoxCollider::BoxCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	_Shape(NULL)
{
	_Type = ShapeType::Box;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
 * @brief	デストラクタ。
 */
BoxCollider::~BoxCollider()
{
	SAFE_DELETE(_Shape);
}
/*!
 * @brief	ボックスコライダーを作成。
 */
void BoxCollider::Create( const Vector3& size )
{
	if (size.x < 0.0f || size.y < 0.0f || size.z < 0.0f) {
		// コライダーサイズに0より小さい値が設定されてるよ。
		abort();
	}
	_Shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
	btVector3 work = _Shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(), work.z());
}

void BoxCollider::Resize(const Vector3& size) {
	SAFE_DELETE(_Shape);
	_Shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
	_collision->GetCollisionObj()->setCollisionShape(_Shape);
	btVector3 work = _Shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(), work.z());
#ifdef _DEBUG
	RecreateViewModel();
#endif
}

#ifdef _DEBUG
void BoxCollider::ColliderModelLoad() {

	//当たり判定を視覚化したオブジェクト生成
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 1);
	_CollisionModel->LoadModel("BoxCollision.X");
}
#endif
