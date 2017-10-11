#include"stdafx.h"
#include "Dungeon.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"

Dungeon::Dungeon(const char * name) :
	GameObject(name)
{

}

void Dungeon::Awake()
{
	//スキンモデル作成
	SkinModel* model = AddComponent<SkinModel>();
	//モデルデータ作成
	SkinModelData* modeldata = new SkinModelData();
	//モデル設定
	modeldata->CloneModelData(SkinModelManager::LoadModel("Dungeon.X"));
	model->SetModelData(modeldata);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);

	transform->SetLocalPosition(Vector3::zero);
	transform->SetLocalScale(Vector3::one);
}

void Dungeon::Start() {
	
}

void Dungeon::Update()
{
}