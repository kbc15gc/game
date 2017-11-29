#pragma once
#include "_Culling\IObjectCulling.h"

//フラスタムカリング処理。
//オブジェクトを内包するAABBを構成する8頂点がカメラに映らない場合にカリングされます。
class CObjectFrustumCulling : public IObjectCulling {
public:
	CObjectFrustumCulling() {}
	~CObjectFrustumCulling() {}
	
	//カリングアルゴリズムの実行。
	//[in] カリングを行いたいオブジェクトを内包するAABB。
	void Execute(const AABB& aabb, const D3DXMATRIX& rotation, const D3DXMATRIX& view, const D3DXMATRIX& proj) override;
};