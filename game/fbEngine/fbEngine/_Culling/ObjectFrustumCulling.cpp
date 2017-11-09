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

void CObjectFrustumCulling::Execute(const AABB & aabb, const D3DXMATRIX& world, const Vector3& sca)
{
	if (_Camera != nullptr) {
		//ビュープロジェクション行列作成。
		auto viewMatrix = _Camera->GetViewMat();
		auto viewProj = _Camera->GetProjectionMat();
		SetCullingFlag(true);
		const float sikii = 1.0f;
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
			//ワールド変換。
			D3DXVec4Transform(&vertPos, &vertPos, &world);
			D3DXVec4Transform(&vertPos, &vertPos, &viewMatrix);

			if (IsCulling()) {
				//行列をかけて、スクリーン座標に変換。
				D3DXVec4Transform(&vertPos, &vertPos, &viewProj);
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
					return;
				}
			}
		}
	
		AABB2D obj, screen;
		obj.SetUpVertex(ru, ld);
		screen.SetUpVertex(Vector2(1, -1), Vector2(-1, 1));
		//2DのAABBの衝突判定。
		if (screen.IsHit(obj))
		{
			SetCullingFlag(false);
			return;
		}

		//3DのAABB。
		btGhostObject coll;
		//形を形成。
		auto hs = aabb.GetHalfSize();
		btBoxShape box(btVector3(hs.x, hs.y, hs.z));
		box.setLocalScaling(btVector3(sca.x, sca.y, sca.z));
		coll.setCollisionShape(&box);
		//移動と回転。
		auto mat = world;
		auto& trans = coll.getWorldTransform();
		trans.setOrigin(btVector3(mat._41, mat._42, mat._43));
		mat._41 = 0.0f;		mat._42 = 0.0f;		mat._43 = 0.0f;
		D3DXQUATERNION q;		D3DXQuaternionRotationMatrix(&q, &mat);
		trans.setRotation(btQuaternion(q.x, q.y, q.z, q.w));

		//当たっているかどうか？
		fbPhysicsCallback::ContactPairTestCallBack cb;
		if(player == nullptr)
			SetPlayer();
		PhysicsWorld::Instance()->GetDynamicWorld()->contactPairTest(player, &coll, cb);
		if (cb.isHit)
			SetCullingFlag(false);
	}
}
