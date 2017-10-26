/**
* 空描画クラスの実装.
*/
#include"fbstdafx.h"
#include"Sky.h"

#include"_Object\_Component\_3D\Light.h"

//#define ON(A,B,C)		\
//void Set##B(A valse)	\
//{ C = valse;}			\
//A Get##B()				\
//{ return C;}

/**
* 初期化.
*/
void Sky::Awake()
{
	_SkyModel = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Sky.X"));
	
	D3DXCreateCubeTextureFromFile(graphicsDevice(), "Asset/Texture/night.dds", &_NightTexture);

	_SkyModel->SetModelData(modeldata);
	_SkyModel->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, false);
	_SkyModel->SetModelEffect(ModelEffectE::CAST_SHADOW, false);
	_SkyModel->SetModelEffect(ModelEffectE::CAST_ENVIRONMENT, true);
	_SkyModel->SetSky(true);
	transform->SetLocalAngle(Vector3(15, 0, 0));

	//太陽の位置.
	_SunPosition.Set(0.0f, 1000000.0f, 0.0f);

	_SunPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_SunPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_SunPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 10.0f);
	_SunPlate->SetSize(Vector2(50.0f, 50.0f));
	_SunPlate->SetBillboard(true);
	_SunPlate->SetActive(false);

	_MoonPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_MoonPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_MoonPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 10.0f);
	_MoonPlate->SetSize(Vector2(30.0f, 30.0f));
	_MoonPlate->SetBillboard(true);
	_MoonPlate->SetActive(false);

	//_SunAngle = D3DXToRadian(270.0f);
}

/**
* 更新.
*/
void Sky::Update()
{
	Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
	if (camera != nullptr)
	{
		const float TMP = 10.0f;
		//太陽の角度を加算.

		//100秒で一日.
		_SunAngle += (PI * 0.01f) * Time::DeltaTime() * TMP;

		//X軸回転.
		_SunPosition.Set(0.0f, sinf(_SunAngle), cosf(_SunAngle));

		//Z軸回転.
		const float angleZ = PI * -0.15f;
		_SunPosition.x = _SunPosition.y * sinf(angleZ);
		_SunPosition.y *= cosf(angleZ);

		//太陽の方向に代入.
		_SunDir = _SunPosition;
		_SunDir.Normalize();
		_SunPosition.Scale(1000000.0f);

		//大気散乱用パラメータを更新.
		_AtomosphereParam.Update(camera->GetPosition(), _SunPosition);

		Light* light = INSTANCE(GameObjectManager)->mainLight;
		if (light != nullptr && light->GetLight().size() > 0)
		{
			//ライトの方向を計算.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);
			limLightDir.Normalize();

			if (_SunDir.Dot(Vector3::up) <= 0.0f)
			{
				light->GetLight()[1]->SetDirection(limLightDir * -1);
			}
			else
			{
				light->GetLight()[1]->SetDirection(Vector3::zero);
			}

			//平行光源の0番目を更新.
			light->GetLight()[0]->SetDirection(limLightDir);
	
			//太陽の位置から環境光を計算.
			float t = max(0.0f, _SunDir.Dot(Vector3::up));
			Vector3 ambientLight;
			ambientLight = Vector3::Lerp(_NightAmbientLight, _DayAmbientLight, t);

			//環境光を設定.
			light->SetAmbientLight(ambientLight);
		}

	}

	transform->SetLocalPosition(camera->GetTarget());
	Vector3 sunModelPos = _SunDir;
	sunModelPos.Scale(2000.0f);
	sunModelPos.Add(camera->GetTarget());
	_SunPlate->transform->SetLocalPosition(sunModelPos);
	Vector3 moonModelPos = _SunDir * -1;
	moonModelPos.Scale(2000.0f);
	moonModelPos.Add(camera->GetTarget());
	_MoonPlate->transform->SetLocalPosition(moonModelPos);

	transform->UpdateTransform();
}

/**
* 描画.
*/
void Sky::Render()
{

}
