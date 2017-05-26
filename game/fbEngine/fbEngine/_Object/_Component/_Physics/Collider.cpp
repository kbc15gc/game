#include "fbstdafx.h"
#include "_Object\_Component\_Physics\Collider.h"
#include "_Object\_GameObject\ModelOject.h"

Collider::~Collider(){
#ifdef _DEBUG
	if (_CollisionModel) {
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);
	}
#endif //_DEBUG
};

void Collider::CreateViewModel(GameObject* Parent, btCollisionObject* CollisionObject, const Vector3& Offset){
	CreateViewModel(Parent,CollisionObject->getWorldTransform(), Offset);
}

void Collider::CreateViewModel(GameObject* Parent, const btTransform& Transform, const Vector3& Offset){
#ifdef _DEBUG

//前に設定されていたアドレスを削除
	if (_CollisionModel)
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);

	// 形状に応じたモデルをロード。
	ColliderModelLoad();

	//モデルがあるのなら
	if (_CollisionModel)
	{
		_CollisionModel->GetSkinModel()->SetModelEffect(ModelEffectE::NONE);
		//半透明な赤に設定。
		_CollisionModel->GetSkinModel()->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
		//子に設定
		if (Parent) {
			_CollisionModel->transform->SetParent(Parent->transform);
		}
		//あたり判定の大きさを調べる。
		btVector3 min, max;
		btTransform tr;
		tr.setIdentity();
		GetBody()->getAabb(tr, min, max);
		Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
		//当たり判定のサイズを指定
		_CollisionModel->transform->SetLocalScale(size);
		//当たり判定をずらす
		_CollisionModel->transform->SetLocalPosition(Offset);
	}
#endif //_DEBUG
}
