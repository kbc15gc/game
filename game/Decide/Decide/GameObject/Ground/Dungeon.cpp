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
	Vector3 StartPos = { 0.0f,70.0f,0.0f };
	transform->SetLocalPosition(StartPos);
	transform->SetLocalScale(Vector3::one);

	//スキンモデル作成
	SkinModel* model = AddComponent<SkinModel>();
	//モデルデータ作成
	SkinModelData* modeldata = new SkinModelData();
	//モデル設定
	modeldata->CloneModelData(SkinModelManager::LoadModel("Dungeon.X"));
	model->SetModelData(modeldata);

	model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);

}

void Dungeon::Start() {
	
}

void Dungeon::Update()
{
}