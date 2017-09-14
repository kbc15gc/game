/**
* 謎の光クラスの実装.
*/
#include"stdafx.h"
#include"MysteryLight.h"

#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* 初期化.
*/
void MysteryLight::Awake()
{
	_LightPlate = INSTANCE(GameObjectManager)->AddNew<Plate>("LightImage", 9);
	_LightPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_LightPlate->transform->SetParent(transform);
	_LightPlate->SetBillboard(true);

	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
}

/**
* 更新.
*/
void MysteryLight::Update()
{

	_LocalTime += Time::DeltaTime();
	if (_LocalTime >= _LightTime)
	{
		_Alpha -= 0.5f * Time::DeltaTime();
		if (_Alpha <= 0.0f)
		{
			SetActive(false, true);
		}
	}

	_LightPlate->transform->SetLocalScale(Vector3::one * fminf(1.0f, _LocalTime) * 0.01f);
	_LightPlate->SetBlendColor(Color(10.0f, 10.0f, 10.0f, _Alpha));
}

void MysteryLight::SetActive(const bool & act, const bool & child)
{
	if (act)
	{
		_Alpha = 1.0f;
		_LocalTime = 0.0f;
		Camera* camera = _PlayerCamera->GetComponent<Camera>();
		Vector3 cameraFoward = camera->GetTarget() - _PlayerCamera->transform->GetPosition();
		cameraFoward.Normalize();
		cameraFoward.Scale(0.5f);
		_LightPlate->transform->SetLocalPosition(_PlayerCamera->transform->GetPosition() + cameraFoward);
	}
	GameObject::SetActive(act, child);
}