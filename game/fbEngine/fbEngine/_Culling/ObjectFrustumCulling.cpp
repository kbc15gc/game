#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"
#include "_Value\AABB.h"

CObjectFrustumCulling::CObjectFrustumCulling()
{
}

CObjectFrustumCulling::~CObjectFrustumCulling()
{
}

void CObjectFrustumCulling::Execute(const AABB & aabb)
{
	if (_Camera != nullptr) {
		//ビュープロジェクション行列作成。
		auto viewProjMatrix = _Camera->GetViewMat() * _Camera->GetProjectionMat();
		SetCullingFlag(true);
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
			//行列をかけて、スクリーン座標に変換。
			D3DXVec4Transform(&vertPos, &vertPos, &viewProjMatrix);
			//正規化座標系に変換。
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;

			if (vertPos.x >= -1.0f
				&& vertPos.x <= 1.0f
				&& vertPos.y >= -1.0f
				&& vertPos.y <= 1.0f
				&& vertPos.z >= 0.0f
				&& vertPos.z <= 1.0f
				) {
				//画面内にいる
				SetCullingFlag(false);
				break;
			}
		}
	}
}
