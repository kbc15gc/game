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
	Vector3 StartPos = { 292.0f ,57.5f ,634.0f };
	transform->SetLocalPosition(StartPos);
	transform->SetLocalScale(Vector3::one);

	//スキンモデル作成
	SkinModel* model = AddComponent<SkinModel>();
	//モデルデータ作成
	SkinModelData* modeldata = new SkinModelData();
	//モデル設定
	modeldata->CloneModelData(SkinModelManager::LoadModel("RockCave.X"));
	model->SetModelData(modeldata);
	//ライト設定。
	MyLight_.SetAmbientLight(Vector4(0.5, 0.5, 0.5, 1));


	model->SetCharacterLight(&MyLight_);

	//model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
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