#include"fbstdafx.h"
#include "BoxCollider.h"
#include "_Object\_GameObject\ModelOject.h"

/*!
 * @brief	コンストラクタ。
 */
BoxCollider::BoxCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	_Shape(NULL)
{
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
	_Shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}

void BoxCollider::ColliderModelLoad() {
	//当たり判定を視覚化したオブジェクト生成
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("BoxCollision.X");
}