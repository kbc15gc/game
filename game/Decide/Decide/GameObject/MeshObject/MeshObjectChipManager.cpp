#include"stdafx.h"
#include "MeshObjectChipManager.h"
#include "MeshObjectChip.h"

namespace
{
	MeshCollisionInfo meshinfo[]
	{
#include "Asset\Collisition\ExportMeshObject.h"
	};
}

MeshObjectChipManager::MeshObjectChipManager(const char * name) :
	GameObject(name)
{

}

void MeshObjectChipManager::Awake()
{
	for (int i = 0; i < sizeof(meshinfo) / sizeof(meshinfo[0]); i++)
	{
		MeshObjectChip* chip = INSTANCE(GameObjectManager)->AddNew<MeshObjectChip>("chip", 1);
		chip->Init(meshinfo->modelName, meshinfo->pos, meshinfo->scale, meshinfo->angle);
	}
}

void MeshObjectChipManager::Start() {

}

void MeshObjectChipManager::Update()
{
}