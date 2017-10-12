#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"

#include "_Value\AABB.h"
#include "_Value\AABB2D.h"

CObjectFrustumCulling::CObjectFrustumCulling()
{
}

CObjectFrustumCulling::~CObjectFrustumCulling()
{
}

void CObjectFrustumCulling::Execute(const AABB & aabb, const D3DXMATRIX& rotation)
{
	if (_Camera != nullptr) {
		//ビュープロジェクション行列作成。
		auto viewProjMatrix = _Camera->GetViewMat() * _Camera->GetProjectionMat();
		SetCullingFlag(true);
		const float sikii = 1.5f;
		Vector2 ru = Vector2(-FLT_MAX, -FLT_MAX), ld = Vector2(FLT_MAX, FLT_MAX);

		//AABBの８頂点をスクリーン空間の正規化座標系に変換する。
		//x、yが1.0～-1.0、zが0.0～1.0の範囲内にいたら画面内にいるということになる。
		//8頂点すべてが画面の外ならカリングする。
		for (int i = 0; i < 8; i++) {
			D3DXVECTOR4 vertPos;
			auto v = aabb.GetVertexPosition(i);
			vertPos.x = v.x;
			vertPos.y = v.y;
			vertPos.z = v.z;
			vertPos.w = 1.0f;
			//回転を考慮。
			//D3DXVec4Transform(&vertPos, &vertPos, &rotation);
			//行列をかけて、スクリーン座標に変換。
			D3DXVec4Transform(&vertPos, &vertPos, &viewProjMatrix);
			//正規化座標系に変換。
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;

			ru.x = max(vertPos.x, ru.x);
			ru.y = max(vertPos.y, ru.y);

			ld.x = min(vertPos.x, ld.x);
			ld.y = min(vertPos.y, ld.y);

			
			if (vertPos.x >= -sikii
				&& vertPos.x <= sikii
				&& vertPos.y >= -sikii
				&& vertPos.y <= sikii
				&& vertPos.z >= 0.0f
				&& vertPos.z <= sikii
				) {
				//画面内にいる
				SetCullingFlag(false);
				break;
			}
		}


		AABB2D obj, screen;
		obj.SetUpVertex(ru, ld);
		screen.SetUpVertex(Vector2(1, -1), Vector2(-1, 1));
		//AABBの衝突判定。
		if (screen.IsHit(obj))
		{
			SetCullingFlag(false);
			return;
		}else
		{
			int a = 0;
		}
	}
}
