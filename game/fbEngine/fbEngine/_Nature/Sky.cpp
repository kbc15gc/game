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
}

/**
* �X�V.
*/
void Sky::Update()
{
	if (_Camera != nullptr)
	{
		//���z�̊p�x�����Z.
		_SunAngle += 0.02f * Time::DeltaTime();

		//X����].
		_SunPosition.Set(0.0f, sinf(_SunAngle), cosf(_SunAngle));

		//Z����].
		float angleZ = PI * -0.15f;
		_SunPosition.x = _SunPosition.y * sinf(angleZ);
		_SunPosition.y *= cosf(angleZ);

		//���z�̕����ɑ��.
		_SunDir = _SunPosition;
		_SunPosition.Scale(1000000.0f);

		//��C�U���p�p�����[�^���X�V.
		_AtomosphereParam.Update(_Camera->GetPosition(), _SunPosition);

		if (_SceneLight != nullptr)
		{
			//���C�g�̕������v�Z.
			Vector3 limLightDir = _SunDir;
			limLightDir.Scale(-1.0f);

			//���s������0�Ԗڂ��X�V.
			_SceneLight->GetLight()[0]->SetDirection(limLightDir);
	
			//���z�̈ʒu����������v�Z.
			float t = max(0.0f, _SunDir.Dot(Vector3::up));
			Vector3 ambientLight;
			ambientLight = Vector3::Lerp(_NightAmbientLight, _DayAmbientLight, t);

			//������ݒ�.
			_SceneLight->SetAmbientLight(ambientLight);
		}

	}

	transform->SetLocalPosition(_Camera->GetTarget());

	transform->UpdateTransform();
}

/**
* �`��.
*/
void Sky::Render()
{

}
