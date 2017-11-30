#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"

#include "_Value\AABB.h"
#include "_Value\AABB2D.h"

void CObjectFrustumCulling::Execute(const AABB & aabb, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	SetCullingFlag(true);
	Vector3 Max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX), Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	//AABBの８頂点をスクリーン空間の正規化座標系に変換する。
	//x、yが1.0～-1.0、zが0.0～1.0の範囲内にいたら画面内にいるということになる。
	//8頂点すべてが画面の外ならカリングする。

	for (int i = 0; i < 8; i++) {

		auto v = aabb.GetVertexPosition(i);
		//頂点座標
		D3DXVECTOR4 vertPos = { v.x,v.y,v.z,1.0f };
		//ワールド座標変換。
		D3DXVec4Transform(&vertPos, &vertPos, &world);
		//ビュー座標変換。
		D3DXVec4Transform(&vertPos, &vertPos, &view);
		//プロジェクション座標変換。
		D3DXVec4Transform(&vertPos, &vertPos, &proj);

		//スクリーン座標に変換。
		vertPos.x /= vertPos.w;
		vertPos.y /= vertPos.w;
		vertPos.z /= vertPos.w;

		Max.x = max(vertPos.x, Max.x);
		Max.y = max(vertPos.y, Max.y);
		Max.z = max(vertPos.z, Max.z);

		Min.x = min(vertPos.x, Min.x);
		Min.y = min(vertPos.y, Min.y);
		Min.z = min(vertPos.z, Min.z);

		if (vertPos.x >= -1.0f
			&& vertPos.x <= 1.0f
			&& vertPos.y >= -1.0f
			&& vertPos.y <= 1.0f
			&& vertPos.z >= 0.0f
			&& vertPos.z <= 1.0f
			) {
			//画面内にいる
			SetCullingFlag(false);
			return;
		}
	}

	AABB2D screen, front, right, under;
	screen.SetUpVertex(Vector2(1, 1), Vector2(-1, -1));
	//前後。
	front.SetUpVertex(Vector2(Max.x, Max.y), Vector2(Min.x, Min.y));
	//左右
	right.SetUpVertex(Vector2(Max.z, Max.y), Vector2(Min.z, Min.y));
	//上下。
	under.SetUpVertex(Vector2(Max.x, Max.z), Vector2(Min.x, Min.z));
	//2DのAABBの衝突判定。
	if (Min.z <= 1.0f &&
		(screen.IsHit(front) ||
		screen.IsHit(right) ||
		screen.IsHit(under)))
	{
			SetCullingFlag(false);
			return;
	}
}
