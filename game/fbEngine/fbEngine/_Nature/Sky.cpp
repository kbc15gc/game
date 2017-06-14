/**
* 空描画クラスの実装.
*/
#include"fbstdafx.h"
#include"Sky.h"

/**
* 初期化.
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
* 更新.
*/
void Sky::Update()
{
	transform->UpdateTransform();
}

/**
* 描画.
*/
void Sky::Render()
{

}
