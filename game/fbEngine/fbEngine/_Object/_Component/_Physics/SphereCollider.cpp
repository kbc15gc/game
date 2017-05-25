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
	shape = new btSphereShape(radius);
}

void SphereCollider::ColliderModelLoad() {
	//当たり判定を視覚化したオブジェクト生成
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("SphereCollision.X");
}