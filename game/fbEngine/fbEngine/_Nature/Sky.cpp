/**
* ��`��N���X�̎���.
*/
#include"fbstdafx.h"
#include"Sky.h"

/**
* ������.
*/
void Sky::Awake()
{
	_SkyModel = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Sky.X"));
	
	_SkyModel->SetModelData(modeldata);
	_SkyModel->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, false);
	_SkyModel->SetModelEffect(ModelEffectE::CAST_SHADOW, false);
	_SkyModel->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT, true);
	_SkyModel->SetSky(true);
	
	transform->SetLocalAngle(Vector3(15, 0, 0));

}

/**
* �X�V.
*/
void Sky::Update()
{
	transform->UpdateTransform();
}

/**
* �`��.
*/
void Sky::Render()
{

}
