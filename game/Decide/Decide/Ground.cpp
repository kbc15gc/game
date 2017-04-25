#include "Ground.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"

#include "fbEngine\_Object\_Component\_3D\Animation.h"


Ground::Ground(const char * name) :
GameObject(name)
{

}

void Ground::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Ground.X"));
	model->SetModelData(modeldata);
	//model->SetModelEffect(ModelEffectE::SPECULAR, false);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	RigidBody* rigid = AddComponent<RigidBody>();
	MeshCollider* mesh = AddComponent<MeshCollider>();

	mesh->Create(model);
	rigid->Create(0, mesh, Collision_ID::GROUND);

	transform->SetLocalPosition(Vector3::zero);
	transform->SetLocalScale(Vector3::one);
}

void Ground::Update()
{

}