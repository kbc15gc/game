#pragma once

class MeshCollider;
class SkinModel;

//メッシュコライダーを管理するマネージャークラス。
class MeshColliderManager
{
public:
	~MeshColliderManager();

	//モデルを受け取り、そのモデルのコライダーを返す。
	static MeshCollider* CloneMeshCollider(const char* filename);
private:
	//メッシュコライダーを保存するmap
	static map<UINT64, MeshCollider*> _Colliders;
};