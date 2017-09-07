/**
* 空描画クラスの実装.
*/
#include"fbstdafx.h"
#include"Sky.h"

#include"_Object\_Component\_3D\Light.h"

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

	//太陽の位置.
	_SunPosition.Set(0.0f, 1000000.0f, 0.0f);
}

/**
* 更新.
*/
void Sky::Update()
{
	if (_Camera != nullptr)
	{
		//太陽の角度を加算.
		_SunAngle += 0.02f * Time::DeltaTime();

		//X軸回転.
		_SunPosition.Set(0.0f, sinf(_SunAngle), cosf(_SunAngle));

		//Z軸回転.
		float angleZ = PI * -0.15f;
		_SunPosition.x = _SunPosition.y * sinf(angleZ);
		_SunPosition.y *= cosf(angleZ);

		//太陽の方向に代入.
		_SunDir = _SunPosition;
		_SunPosition.Scale(1000000.0f);

		//大気散乱用パラメータを更新.
		_AtomosphereParam.Update(_Camera->GetPosition(), _SunPosition);

		if (_SceneLight != nullptr && _SceneLight->GetLight().size() > 0)
		{
			//ライトの方向を計算.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);

			//平行光源の0番目を更新.
			_SceneLight->GetLight()[0]->SetDirection(limLightDir);
	
			//太陽の位置から環境光を計算.
			float t = max(0.0f, _SunDir.Dot(Vector3::up));
			Vector3 ambientLight;
			ambientLight = Vector3::Lerp(_NightAmbientLight, _DayAmbientLight, t);

			//環境光を設定.
			_SceneLight->SetAmbientLight(ambientLight);
		}

	}

	transform->SetLocalPosition(_Camera->GetTarget());

	transform->UpdateTransform();
}

/**
* 描画.
*/
void Sky::Render()
{

}
