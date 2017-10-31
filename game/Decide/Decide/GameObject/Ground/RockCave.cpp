#include"stdafx.h"
#include "RockCave.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"

RockCave::RockCave(const char * name) :
	GameObject(name)
{

}

void RockCave::Awake()
{
	Vector3 StartPos = { 297.408051f ,50.0f ,634.428467f };
	transform->SetLocalPosition(StartPos);
	transform->SetLocalScale(Vector3::one);

	//スキンモデル作成
	SkinModel* model = AddComponent<SkinModel>();
	//モデルデータ作成
	SkinModelData* modeldata = new SkinModelData();
	//モデル設定
	modeldata->CloneModelData(SkinModelManager::LoadModel("RockCave.X"));
	model->SetModelData(modeldata);

	model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);

}

void RockCave::Start() {

}

void RockCave::Update()
{
}