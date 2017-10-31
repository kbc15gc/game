/**
* ‹ó•`‰æƒNƒ‰ƒX‚ÌÀ‘•.
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
* ‰Šú‰».
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

	//‘¾—z‚ÌˆÊ’u.
	_SunPosition.Set(0.0f, 1000000.0f, 0.0f);

	_SunPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_SunPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_SunPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 10.0f);
	_SunPlate->SetSize(Vector2(50.0f, 50.0f));
	_SunPlate->SetBillboard(true);
	_SunPlate->SetActive(false);

	_MoonPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_MoonPlate->SetTexture(LOADTEXTURE("moon0015.png"));
	_MoonPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 1.0f);
	Vector2 size = _MoonPlate->GetComponent<PlatePrimitive>()->GetSize();
	_MoonPlate->SetSize(size * 0.5f);
	_MoonPlate->SetBillboard(true);
	_MoonPlate->SetActive(false);

	//_SunAngle = D3DXToRadian(270.0f);
}

/**
* XV.
*/
void Sky::Update()
{
	Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
	if (camera != nullptr)
	{
		const float TMP = 10.0f;
		//‘¾—z‚ÌŠp“x‚ğ‰ÁZ.

		//100•b‚Åˆê“ú.
		_SunAngle += (PI * 0.01f) * Time::DeltaTime() * TMP;

		//X²‰ñ“].
		_SunPosition.Set(0.0f, sinf(_SunAngle), cosf(_SunAngle));

		//Z²‰ñ“].
		const float angleZ = PI * -0.15f;
		_SunPosition.x = _SunPosition.y * sinf(angleZ);
		_SunPosition.y *= cosf(angleZ);

		//‘¾—z‚Ì•ûŒü‚É‘ã“ü.
		_SunDir = _SunPosition;
		_SunDir.Normalize();
		_SunPosition.Scale(1000000.0f);

		//‘å‹CU——pƒpƒ‰ƒ[ƒ^‚ğXV.
		_AtomosphereParam.Update(camera->GetPosition(), _SunPosition);

		Light* light = INSTANCE(GameObjectManager)->mainLight;
		if (light != nullptr && light->GetLight().size() > 0)
		{
			//ƒ‰ƒCƒg‚Ì•ûŒü‚ğŒvZ.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);
			limLightDir.Normalize();

			light->GetLight()[1]->SetDirection(limLightDir * -1);
			float MoonColor = max(0.0f, _SunDir.Dot(Vector3::up) * -1) * 0.5f;
			light->GetLight()[1]->SetColor(Color(MoonColor, MoonColor, MoonColor, 10.0f));

			//•½sŒõŒ¹‚Ì0”Ô–Ú‚ğXV.
			light->GetLight()[0]->SetDirection(limLightDir);
	
			//‘¾—z‚ÌˆÊ’u‚©‚çŠÂ‹«Œõ‚ğŒvZ.
			float t = max(0.0f, _SunDir.Dot(Vector3::up));
			Vector3 ambientLight;
			ambientLight = Vector3::Lerp(_NightAmbientLight, _DayAmbientLight, t);

			//ŠÂ‹«Œõ‚ğİ’è.
			light->SetAmbientLight(ambientLight);
		}

	}

	transform->SetLocalPosition(camera->GetTarget());

	float angle = _SunDir.Dot(Vector3::up);
	if (angle > 0.0f)
	{
		_ShadowLightPosition = _SunDir;
	}
	else
	{
		_ShadowLightPosition = _SunDir * -1;
	}

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
* •`‰æ.
*/
void Sky::Render()
{

}
