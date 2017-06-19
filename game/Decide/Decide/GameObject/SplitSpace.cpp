#include "stdafx.h"
#include "SplitSpace.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "GameSystem.h"
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

void SplitSpace::Split(const SkinModelData* data,const Transform& transform, int x, int y, int z) {
	if (data == nullptr) {
		abort();
		// モデルデータがnull。
	}

	CreateSplitBox(CreateSpaceBox(*data, _unSplitSpaceSize),transform,x,y,z);
}

const Vector3& SplitSpace::CreateSpaceBox(const SkinModelData& data, Vector3& size) {
	float minX,minY,minZ,maxX,maxY,maxZ;	// 頂点の最小位置と最大位置(モデルを内包できるボックスを定義するので、要素ごとに最小値と最大値を求める)。
	minX = minY = minZ = FLT_MAX;
	maxX = maxY = maxZ = 0.0f;
	
	vector<LPD3DXMESH> meshList = data.GetMeshList();	// すべてのメッシュを取得。
	for (auto mesh : meshList) {
		// すべてのメッシュを参照する。

		int offset = 0;	// 頂点データの先頭から特定データまでの差分。
		int stride = 0;	// 頂点一つ分のバイトサイズ。
		D3DXVECTOR3* localPosition;	// モデルの座標系での頂点位置。

		LPDIRECT3DVERTEXBUFFER9 vb = nullptr;
		mesh->GetVertexBuffer(&vb);	// 頂点バッファ取得。 

		D3DVERTEXBUFFER_DESC desc;
		vb->GetDesc(&desc);	// 頂点バッファに関する情報を取得。
		stride = desc.Size / mesh->GetNumVertices();	// 頂点一つ分のバイトサイズを取得(頂点バッファのバイトサイズ / 頂点数)。

		D3DVERTEXELEMENT9 element[MAX_FVF_DECL_SIZE];	// 頂点データのレイアウト情報はD3DVERTEXELEMENT9構造体の配列で表される。
		mesh->GetDeclaration(element);	// 頂点の構成要素情報を取得。
		for (int idx = 0; true;idx++) {
			// 頂点データからお望みのデータまでのOffsetを探索。

			if (element[idx].Usage == D3DDECLUSAGE_POSITION) {
				// お望みのデータ発見。
				offset = element[idx].Offset;
				break;
			}
		}

		char* pData = nullptr;	// 1バイト単位で使用できるchar*でデータを取得。
		vb->Lock(0,0,(void**)&pData, D3DLOCK_READONLY);	// 頂点バッファをロック。※引数は暫定、とりあえず読み取り専用。
		for (int idx = 0; idx < mesh->GetNumVertices(); idx++) {
			// 頂点すべてを参照する。

			localPosition = reinterpret_cast<D3DXVECTOR3*>(pData + offset);	// 各頂点データの先頭から位置情報の入ったデータまで差分を加算。
			if (localPosition->x < minX) {
				minX = localPosition->x;
			}
			else if (localPosition->x > maxX) {
				maxX = localPosition->x;
			}

			if (localPosition->y < minY) {
				minY = localPosition->y;
			}
			else if (localPosition->y > maxY) {
				maxY = localPosition->y;
			}

			if (localPosition->z < minZ) {
				minZ = localPosition->z;
			}
			else if (localPosition->z > maxZ) {
				maxZ = localPosition->z;
			}

			pData += stride;	// 1頂点分ポインタを進める。
		}
		vb->Unlock();	// 頂点バッファのロックを解除。
	}

	// 求めた最小位置と最大位置からボックスのサイズを定義。
	size = Vector3(maxX - minX,maxY - minY,maxZ - minZ);
	return size;
}

void SplitSpace::CreateSplitBox(const Vector3& size,const Transform& transform, int x, int y, int z) {
	if (x <= 0 || y <= 0 || z <= 0) {
		abort();
		// 分割数に0より小さい値が設定された。
	}

	for (int idxX = 0; idxX < x; idxX++) {
		for (int idxY = 0; idxY < y; idxY++) {
			for (int idxZ = 0; idxZ < z; idxZ++) {
				// 分割数分のコリジョン生成。

				CollisionObject* box = INSTANCE(GameObjectManager)->AddNew<CollisionObject>("SpaceBox", 1);
				Vector3 splitSize(size.x / x, size.y / y, size.z / z);
				box->Initialize(Collision_ID::SPACE, splitSize);
				// 元の位置情報を中心として分割できるようポジション調整。
				box->transform->SetPosition(transform.GetPosition() - (size * 0.5f) + Vector3((idxX * (splitSize.x/* + 1.0f*/)) + (splitSize.x * 0.5f), (idxY * (splitSize.y/* + 1.0f*/)) + (splitSize.y * 0.5f), (idxZ * (splitSize.z/* + 1.0f*/)) + (splitSize.z * 0.5f)));
				box->transform->SetRotation(transform.GetRotation());
				box->transform->SetScale(transform.GetScale());

				_SpaceCollisions.push_back(box);
			}
		}
	}
}
