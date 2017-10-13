#pragma once

#include "_Culling\IObjectCulling.h"
class Camera;


//フラスタムカリング処理。
//オブジェクトを内包するAABBを構成する8頂点がカメラに映らない場合にカリングされます。
class CObjectFrustumCulling : public IObjectCulling {
public:
	CObjectFrustumCulling();
	~CObjectFrustumCulling();
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
};