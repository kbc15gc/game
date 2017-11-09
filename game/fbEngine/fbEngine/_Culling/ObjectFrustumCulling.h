#pragma once

#include "_Culling\IObjectCulling.h"
class Camera;


//フラスタムカリング処理。
//オブジェクトを内包するAABBを構成する8頂点がカメラに映らない場合にカリングされます。
class CObjectFrustumCulling : public IObjectCulling {
public:
	CObjectFrustumCulling();
	~CObjectFrustumCulling();
	void SetPlayer()
	{
		auto obj = INSTANCE(GameObjectManager)->FindObject("Player");
		if (obj != nullptr)
			player = obj->GetComponent<RigidBody>()->GetCollisionObj();
	}
	//カメラを設定。
	void SetCamera(const Camera& camera)
	{
		_Camera = &camera;
	}
	//カリングアルゴリズムの実行。
	//[in] カリングを行いたいオブジェクトを内包するAABB。
	void Execute(const AABB& aabb, const D3DXMATRIX& rotation) override;
private:
	const Camera*	_Camera = nullptr;

	btCollisionObject* player;
};