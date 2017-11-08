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
	//��v������̂Ȃ�
	if (_Colliders[hush] == nullptr)
	{
		coll = new MeshCollider();
		//���f������R���C�_�[����B
		coll->Create(SkinModelManager::LoadModel(filename));

		_Colliders[hush] = coll;
	}
	else
	{
		coll = _Colliders[hush];
	}

	return coll;
}