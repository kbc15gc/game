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
		MeshObjectChip* meshchip = INSTANCE(GameObjectManager)->AddNew<MeshObjectChip>("meshchip", 1);
		meshchip->Init(meshinfo->modelName, meshinfo->pos, meshinfo->scale, meshinfo->angle);
	}
}

void MeshObjectChipManager::Start() {

}

void MeshObjectChipManager::Update()
{
}