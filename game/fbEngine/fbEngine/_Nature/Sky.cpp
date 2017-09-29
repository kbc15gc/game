/**
* ��`��N���X�̎���.
*/
#include"fbstdafx.h"
#include"Sky.h"

#include"_Object\_Component\_3D\Light.h"

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

	//���z�̈ʒu.
	_SunPosition.Set(0.0f, 1000000.0f, 0.0f);

	_SunPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_SunPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_SunPlate->GetComponent<PlatePrimitive>()->SetBlendColor(Color::white * 1.5f);
	_SunPlate->SetBillboard(true);
	_SunPlate->SetActive(false);
}

/**
* �X�V.
*/
void Sky::Update()
{
	Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
	if (camera != nullptr)
	{
		const float TMP = 5.0f;
		//���z�̊p�x�����Z.
		_SunAngle += 0.02f * Time::DeltaTime() * TMP;

		//X����].
		_SunPosition.Set(0.0f, sinf(_SunAngle), cosf(_SunAngle));

		//Z����].
		const float angleZ = PI * -0.15f;
		_SunPosition.x = _SunPosition.y * sinf(angleZ);
		_SunPosition.y *= cosf(angleZ);

		//���z�̕����ɑ��.
		_SunDir = _SunPosition;
		_SunPosition.Scale(1000000.0f);

		//��C�U���p�p�����[�^���X�V.
		_AtomosphereParam.Update(camera->GetPosition(), _SunPosition);


		Light* light = INSTANCE(GameObjectManager)->mainLight;
		if (light != nullptr && light->GetLight().size() > 0)
		{
			//���C�g�̕������v�Z.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);
			limLightDir.Normalize();

			//���s������0�Ԗڂ��X�V.
			light->GetLight()[0]->SetDirection(limLightDir);
	
			//���z�̈ʒu����������v�Z.
			float t = max(0.0f, _SunDir.Dot(Vector3::up));
			Vector3 ambientLight;
			ambientLight = Vector3::Lerp(_NightAmbientLight, _DayAmbientLight, t);

			//������ݒ�.
			light->SetAmbientLight(ambientLight);
		}

	}

	transform->SetLocalPosition(camera->GetTarget());
	Vector3 sunModelPos = _SunDir;
	sunModelPos.Scale(2000.0f);
	sunModelPos.Add(camera->GetTarget());
	_SunPlate->transform->SetLocalPosition(sunModelPos);

	transform->UpdateTransform();
}

/**
* �`��.
*/
void Sky::Render()
{

}
