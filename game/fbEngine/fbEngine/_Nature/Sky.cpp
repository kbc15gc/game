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
	_MoonPlate->SetTexture(LOADTEXTURE("moon0015.png"));
	_MoonPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 1.3f);
	Vector2 size = _MoonPlate->GetComponent<PlatePrimitive>()->GetSize();
	_MoonPlate->SetSize(size * 0.5f);
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
		const float TMP = 0.01f / 6.0f * 100;
		//太陽の角度を加算.

		//100秒で一日.
		_SunAngle += PI * Time::DeltaTime() * TMP;

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

		float angle = _SunDir.Dot(Vector3::up);
		if (angle > 0.0f)
		{
			_ShadowLightPosition = _SunDir;
		}
		else
		{
			_ShadowLightPosition = _SunDir * -1;
		}

		//大気散乱用パラメータを更新.
		_AtomosphereParam.Update(camera->GetPosition(), _SunPosition);

		Light* light = INSTANCE(GameObjectManager)->mainLight;
		if (light != nullptr && light->GetLight().size() > 0)
		{
			//ライトの方向を計算.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);
			limLightDir.Normalize();

			light->GetLight()[1]->SetDirection(limLightDir * -1);
			float MoonColor = max(0.0f, _SunDir.Dot(Vector3::up) * -1) * 0.5f;
			light->GetLight()[1]->SetColor(Color(MoonColor, MoonColor, MoonColor, 10.0f));

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

	Vector3 MoonPos = _SunDir * -1;
	MoonPos.Scale(2000.0f);
	MoonPos.Add(camera->GetTarget());
	_MoonPlate->transform->SetLocalPosition(MoonPos);

	transform->UpdateTransform();
}

/**
* 描画.
*/
void Sky::Render()
{

}

AtmosphericScatteringParamS::AtmosphericScatteringParamS()
{
	fScaleDepth = 0.25;
	const float fInvScaleDepth = 4;

	const int nSamples = 2;
	const float fSamples = 2.0f;
	const float km = 0.0010f;
	const float ESun = 30.0f;
	const float kr = 0.0025f;
	//大気錯乱パラメータの更新。
	fKm4PI = km * 4.0f * PI;
	fKmESun = km * ESun;

	fKr4PI = kr * 4.0f * PI;
	fKrESun = kr * ESun;
	//fInnerRadius = 8.0f; //単位km
	fInnerRadius = 8.0f; //単位km
	fInnerRadius2 = fInnerRadius * fInnerRadius;
	fOuterRadius = fInnerRadius + (fInnerRadius * fScaleDepth * 0.4f);
	fOuterRadius2 = fOuterRadius * fOuterRadius;

	fScale = 1.0f / (fOuterRadius - fInnerRadius);
	fScaleOverScaleDepth = (1.0f / (fOuterRadius - fInnerRadius)) / fScaleDepth;
	g = -0.990f;
	g2 = g * g;
	float fWavelength[3], fWavelength4[3];
	fWavelength[0] = 0.680f;		// 650 nm for red
	fWavelength[1] = 0.570f;		// 570 nm for green
	fWavelength[2] = 0.475f;		// 475 nm for blue
	fWavelength4[0] = powf(fWavelength[0], 4.0f);
	fWavelength4[1] = powf(fWavelength[1], 4.0f);
	fWavelength4[2] = powf(fWavelength[2], 4.0f);
	v3InvWavelength.Set(1 / fWavelength4[0], 1 / fWavelength4[1], 1 / fWavelength4[2]);
}