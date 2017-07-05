#include "stdafx.h"
#include "SplitSpace.h"
#include "fbEngine\_Object\_Component\_Physics\BoxCollider.h"
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "GameSystem.h"
#include "SpaceCollisionObject.h"

const int SplitSpace::_defaultAttr = static_cast<int>(fbCollisionAttributeE::ALL);

void SplitSpace::Awake() {
	// 隣接要素を列挙。
	_AdjacentIndex[Space::Up] = Vector3::up;
	_AdjacentIndex[Space::Down] = Vector3::down;
	_AdjacentIndex[Space::Right] = Vector3::right;
	_AdjacentIndex[Space::Left] = Vector3::left;
	_AdjacentIndex[Space::Front] = Vector3::front;
	_AdjacentIndex[Space::Back] = Vector3::back;

	_AdjacentIndex[Space::RightUp] = Vector3::up + Vector3::right;
	_AdjacentIndex[Space::RightDown] = Vector3::down + Vector3::right;
	_AdjacentIndex[Space::RightFront] = Vector3::right + Vector3::front;
	_AdjacentIndex[Space::RightBack] = Vector3::right + Vector3::back;

	_AdjacentIndex[Space::LeftUp] = Vector3::up + Vector3::left;
	_AdjacentIndex[Space::LeftDown] = Vector3::down + Vector3::left;
	_AdjacentIndex[Space::LeftFront] = Vector3::left + Vector3::front;
	_AdjacentIndex[Space::LeftBack] = Vector3::left + Vector3::back;

	_AdjacentIndex[Space::UpFront] = Vector3::up + Vector3::front;
	_AdjacentIndex[Space::UpBack] = Vector3::up + Vector3::back;
	_AdjacentIndex[Space::DownFront] = Vector3::down + Vector3::front;
	_AdjacentIndex[Space::DownBack] = Vector3::down + Vector3::back;

	_AdjacentIndex[Space::RightUpFront] = _AdjacentIndex[Space::RightUp] + Vector3::front;
	_AdjacentIndex[Space::RightDownFront] = _AdjacentIndex[Space::RightDown] + Vector3::front;
	_AdjacentIndex[Space::LeftUpFront] = _AdjacentIndex[Space::LeftUp] + Vector3::front;
	_AdjacentIndex[Space::LeftDownFront] = _AdjacentIndex[Space::LeftDown] + Vector3::front;
	_AdjacentIndex[Space::RightUpBack] = _AdjacentIndex[Space::RightUp] + Vector3::back;
	_AdjacentIndex[Space::RightDownBack] = _AdjacentIndex[Space::RightDown] + Vector3::back;
	_AdjacentIndex[Space::LeftUpBack] = _AdjacentIndex[Space::LeftUp] + Vector3::back;
	_AdjacentIndex[Space::LeftDownBack] = _AdjacentIndex[Space::LeftDown] + Vector3::back;
}

void SplitSpace::Update() {
	if (_nowHitSpace == nullptr) {
		// プレイヤーがどの空間にもあたっていない。

		// すべての空間から探索。
		ChangeActivateSpace(IsHitPlayerToAll());
	}
	else if (_nowHitSpace->isHitPlayer()) {
		// 一瞬前まで衝突していた空間にプレイヤーがまだ衝突している。
		// ※今のところ何もしない。
	}
	else {
		// 一瞬前まで衝突していた空間からプレイヤーが外れた。

		// 隣接している空間を探索。
		SpaceCollisionObject* hit = _nowHitSpace->IsHitPlayerAdjacent();
		if (hit) {
			// 衝突している空間発見。

			ChangeActivateSpace(hit);
		}
		else {
			// ロスト。

			// すべての空間から探索。
			ChangeActivateSpace(IsHitPlayerToAll());
		}
	}
}

#ifdef _DEBUG
void SplitSpace::Debug() {
	if ((KeyBoardInput->isPush(DIK_N))) {
		if (GetIsStopUpdate()) {
			for (auto& x : _SpaceCollisions) {
				for (auto& y : x) {
					for (auto z : y) {
						z->GetCollision()->enable = true;
						//z->GetCollider()->RenderEnable();
						z->GetCollider()->enable = true;
					}
				}
			}
			// 空間分割オン。
			for (auto& x : _SpaceCollisions) {
				for (auto& y : x) {
					for (auto z : y) {
						z->DisableNotAdjacent(nullptr);
					}
				}
			}

			SetIsStopUpdate(false);
		}
		else {
			// 空間分割オフ。
			for (auto& x : _SpaceCollisions) {
				for (auto& y : x) {
					for (auto z : y) {
						z->EhableThisAndAdjacent();
					}
				}
			}
			for (auto& x : _SpaceCollisions) {
				for (auto& y : x) {
					for (auto z : y) {
						z->GetCollision()->enable = false;
						z->GetCollider()->RenderDisable();
						z->GetCollider()->enable = false;
					}
				}
			}

			_nowHitSpace = nullptr;

			SetIsStopUpdate(true);
		}
	}
}
#endif

void SplitSpace::Split(const SkinModelData* data, Transform* transform, int x, int y, int z, int attr,const Vector3& offset) {
	if (data == nullptr) {
		abort();
		// モデルデータがnull。
	}

	_splitX = x;
	_splitY = y;
	_splitZ = z;

	CreateSplitBox(CreateSpaceBox(*data,*transform, _unSplitSpaceSize),transform,x,y,z,attr,offset);
}

const Vector3& SplitSpace::CreateSpaceBox(const SkinModelData& data, const Transform& transform,Vector3& size) {
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
			D3DXVECTOR4 worldPos;

			// 頂点座標はローカルポジションなのでワールド座標に変換。
			D3DXMATRIX mat = transform.GetWorldMatrix();
			D3DXVec3Transform(&worldPos, localPosition, &mat);

			if (worldPos.x < minX) {
				minX = worldPos.x;
			}
			else if (worldPos.x > maxX) {
				maxX = worldPos.x;
			}

			if (worldPos.y < minY) {
				minY = worldPos.y;
			}
			else if (worldPos.y > maxY) {
				maxY = worldPos.y;
			}

			if (worldPos.z < minZ) {
				minZ = worldPos.z;
			}
			else if (worldPos.z > maxZ) {
				maxZ = worldPos.z;
			}

			pData += stride;	// 1頂点分ポインタを進める。
		}
		vb->Unlock();	// 頂点バッファのロックを解除。
	}

	// 求めた最小位置と最大位置からボックスのサイズを定義。
	size = Vector3(maxX - minX,maxY - minY,maxZ - minZ);
	return size;
}

void SplitSpace::CreateSplitBox(const Vector3& size, Transform* transform, int x, int y, int z, int attr,const Vector3& offset) {
	if (x <= 0 || y <= 0 || z <= 0) {
		abort();
		// 分割数に0より小さい値が設定された。
	}

	_splitSpaceSize = Vector3(size.x / static_cast<float>(x), size.y / static_cast<float>(y), size.z / static_cast<float>(z));

	// 三次元配列分メモリ領域確保。
	_SpaceCollisions = vector<vector<vector<SpaceCollisionObject*>>>(x, vector<vector<SpaceCollisionObject*>>(y, vector<SpaceCollisionObject*>(z, nullptr)));

	for (int idxX = 0; idxX < x; idxX++) {
		for (int idxY = 0; idxY < y; idxY++) {
			for (int idxZ = 0; idxZ < z; idxZ++) {
				// 分割数分のコリジョン生成。

				SpaceCollisionObject* box = INSTANCE(GameObjectManager)->AddNew<SpaceCollisionObject>("SpaceBox", System::MAX_PRIORITY);
				// 元の位置情報を中心として分割できるようポジション調整。
				Vector3 pos = Vector3::zero;
				pos.x = (pos.x - (size.x * 0.5f)) + (idxX * _splitSpaceSize.x) + (_splitSpaceSize.x * 0.5f) + offset.x;
				pos.y = (pos.y - (size.y * 0.5f)) + (idxY * _splitSpaceSize.y) + (_splitSpaceSize.y * 0.5f) + offset.y;
				pos.z = (pos.z - (size.z * 0.5f)) + (idxZ * _splitSpaceSize.z) + (_splitSpaceSize.z * 0.5f) + offset.z;

				box->Create(pos, Quaternion::Identity, _splitSpaceSize, Collision_ID::SPACE, transform, attr,Vector3(static_cast<float>(idxX), static_cast<float>(idxY), static_cast<float>(idxZ)));
				_SpaceCollisions[idxX][idxY][idxZ] = box;
			}
		}
	}

	// 周囲の空間コリジョンを登録していく。
	_AdjacentSpace();
}

void SplitSpace::_AdjacentSpace() {
	for (int x = 0; x < _splitX; x++) {
		for (int y = 0; y < _splitY; y++) {
			for (int z = 0; z < _splitZ; z++) {
				for (int idx = 0; idx < Space::Max; idx++) {
					int workX = x + static_cast<int>(_AdjacentIndex[idx].x);
					int workY = y + static_cast<int>(_AdjacentIndex[idx].y);
					int workZ = z + static_cast<int>(_AdjacentIndex[idx].z);

					if (workX >= 0 && workX < _splitX && workY >= 0 && workY < _splitY && workZ >= 0 && workZ < _splitZ) {
						// 配列外にアクセスしていない。
						_SpaceCollisions[x][y][z]->AddAdjacentSpaceObject(_SpaceCollisions[workX][workY][workZ]);
					}
				}
			}
		}
	}
}

void SplitSpace::AddObjectHitSpace(const GameObject& object) {
	for (auto& x : _SpaceCollisions) {
		for (auto& y : x) {
			for (auto z : y) {
				z->AddObjectHitSpace(const_cast<GameObject&>(object));
			}
		}
	}
}

void SplitSpace::RegistrationObject() {
	for (auto& x : _SpaceCollisions) {
		for (auto& y : x) {
			for (auto z : y) {
				z->RegistrationObject();
			}
		}
	}
}

SpaceCollisionObject* SplitSpace::IsHitPlayerToAll() {
	for (auto& x : _SpaceCollisions) {
		for (auto& y : x) {
			for (auto z : y) {
				if (z->isHitPlayer()) {
					// 衝突している。
					return z;
				}
			}
		}
	}
	return nullptr;
}

void SplitSpace::ChangeActivateSpace(SpaceCollisionObject* Obj) {
	if (Obj) {
		if (_nowHitSpace) {
			// 以前のアクティブ空間の中で、新しい衝突空間と隣接していない空間を非アクティブ化。
			_nowHitSpace->DisableNotAdjacent(Obj);
		}

		// 新しい衝突空間と隣接空間に属するオブジェクトをアクティブ化。
		Obj->EhableThisAndAdjacent();

		// 衝突している空間を更新。
		_nowHitSpace = Obj;
	}
}