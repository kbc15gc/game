#include "fbstdafx.h"
#include "MeshColliderManager.h"
#include "_Object\_Component\_Physics\MeshCollider.h"
#include "_Object\_Component\_3D\SkinModel.h"

map<UINT64, MeshCollider*> MeshColliderManager::_Colliders;

MeshColliderManager::~MeshColliderManager()
{
}

MeshCollider * MeshColliderManager::CloneMeshCollider(const char * filename)
{
	MeshCollider* coll;
	

	UINT64 hush = Support::MakeHash(filename);
	//一致するものなし
	if (_Colliders[hush] == nullptr)
	{
		coll = new MeshCollider();
		//モデルからコライダー制作。
		coll->Create(SkinModelManager::LoadModel(filename));

		_Colliders[hush] = coll;
	}
	else
	{
		coll = _Colliders[hush];
	}

	return coll;
}