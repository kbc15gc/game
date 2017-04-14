#include "FireChip.h"
#include "fbEngine/Skinmodel.h"
#include "fbEngine/SkinmodelData.h"
#include "fbEngine/Light.h"

FireChip::FireChip(const char * name) :
	GameObject(name)
{

}

void FireChip::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("FireChip.X"));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	transform->SetLocalPosition(Vector3(30.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);
}

void FireChip::Update()
{
	static Vector3 angle(0.0f, 0.0f, 0.0f);
	angle.y += 2.0f;
	transform->SetLocalAngle(angle);
}