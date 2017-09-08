#include "fbstdafx.h"
#include "_Object\_Component\_Physics\Collider.h"
#include "_Object\_GameObject\ModelOject.h"

#ifdef _DEBUG
const wchar_t* Collider::ShapeName[] = {
	L"Box",
	L"Sphere",
	L"Capsule",
	L"Mesh",
};
#endif


Collider::~Collider(){
#ifdef _DEBUG
	if (_CollisionModel) {
		_CollisionModel->transform->SetParent(nullptr);
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);
	}
#endif //_DEBUG
};

#ifdef _DEBUG
void Collider::Debug(){
	if ((KeyBoardInput->isPush(DIK_M))) {
		if (GetIsRender()) {
			// コリジョン描画オフ。
			RenderDisable();
		}
		else {
			// コリジョン描画オン。
			RenderEnable();
		}
	}
}
#endif

#ifdef _DEBUG
void Collider::RecreateViewModel() {
	bool isEnable = _CollisionModel->GetSkinModel()->enable;
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
	_CollisionModel->GetSkinModel()->enable = isEnable;	// 直前の描画状態を設定。
}

void Collider::CreateViewModel(const btTransform& collisionTr){

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
		//あたり判定の大きさを調べる。
		btVector3 min, max;
		btTransform tr;
		tr.setIdentity();
		GetBody()->getAabb(tr, min, max);
		Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
		//当たり判定のサイズを指定
		_CollisionModel->transform->SetLocalScale(size);
		//実際のコリジョンを親に設定(描画用モデルをコリジョンの座標系にする。)

		// コリジョンのTransform情報生成。
		_CollisionTr.reset(new Transform(nullptr,nullptr));
		
		UpdateTransform(collisionTr);

		//コリジョン描画用モデルの中心点とコリジョンの中心点の差分を設定。
		_CollisionModel->transform->SetLocalPosition(_CollisionModelOffset);
	}
}


void Collider::UpdateTransform(const btTransform& collisionTr) {

	if (_CollisionTr.get()) {
		// 位置情報設定。
		btVector3 pos = collisionTr.getOrigin();
		_CollisionTr->SetPosition(pos.getX(), pos.getY(), pos.getZ());
		// 回転情報設定。
		btQuaternion rot = collisionTr.getRotation();
		_CollisionTr->SetRotation(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
		// コリジョンのTransform情報を親に設定。
		_CollisionModel->transform->SetParent(_CollisionTr.get());
	}
}


bool Collider::GetIsRender() {
	if (_CollisionModel) {
		return _CollisionModel->GetSkinModel()->enable;
	}
	return false;
}

void Collider::RenderEnable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->enable = true;
	}
}

void Collider::RenderDisable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->enable = false;
	}
}
#endif