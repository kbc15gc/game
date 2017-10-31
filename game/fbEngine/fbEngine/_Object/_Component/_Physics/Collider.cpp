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
	bool isEnable = _CollisionModel->GetSkinModel()->GetEnable();
	CreateViewModel();
	_CollisionModel->GetSkinModel()->SetEnable(isEnable);	// 直前の描画状態を設定。
}

void Collider::CreateViewModel(){

//前に設定されていたアドレスを削除
	if (_CollisionModel)
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);

	_CollisionModel = nullptr;

	// 形状に応じたモデルをロード。
	ColliderModelLoad();

	//モデルがあるのなら
	if (_CollisionModel)
	{
		_CollisionModel->GetSkinModel()->SetModelEffect(ModelEffectE::NONE);
		//半透明な赤に設定。
		_CollisionModel->GetSkinModel()->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
		//あたり判定の大きさを調べる。

		//btVector3 min, max;
		//btTransform tr;
		//tr.setIdentity();
		//GetBody()->getAabb(tr, min, max);
		//Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());

		Vector3 size = _halfSize * 2.0f;


		//当たり判定のサイズを指定
		//_CollisionModel->transform->SetLocalScale(size);
		_CollisionModel->transform->SetScale(size);

		//実際のコリジョンを親に設定(描画用モデルをコリジョンの座標系にする。)

		// コリジョンのTransform情報生成。
		//_CollisionTr.reset(new Transform(nullptr,nullptr));
		
		UpdateTransform();

		////コリジョン描画用モデルの中心点とコリジョンの中心点の差分を設定。
		//_CollisionModel->transform->SetLocalPosition(_CollisionModelOffset);
	}
}


void Collider::UpdateTransform() {

	if (_CollisionModel) {
		//_CollisionModel->transform->SetParent(transform);

		// 位置情報設定。
		_CollisionModel->transform->SetLocalPosition(_collision->GetOffset());
	}
}


bool Collider::GetIsRender() {
	if (_CollisionModel) {
		return _CollisionModel->GetSkinModel()->GetEnable();
	}
	return false;
}

void Collider::RenderEnable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->SetEnable(true);
	}
}

void Collider::RenderDisable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->SetEnable(false);
	}
}
#endif