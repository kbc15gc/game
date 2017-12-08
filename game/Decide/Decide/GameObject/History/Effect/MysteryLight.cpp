/**
* 謎の光クラスの実装.
*/
#include"stdafx.h"
#include"MysteryLight.h"

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"../HistoryManager.h"

/**
* 初期化.
*/
void MysteryLight::Awake()
{
	_LightPlate = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LightImage", 10);
	_LightPlate->SetTexture(LOADTEXTURE("UI/circle128.png"));
	_LightPlate->transform->SetParent(transform);
	_LightPlate->transform->SetLocalScale(Vector3::zero);

	transform->SetLocalPosition(g_WindowSize / 2);

	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
}

/**
* 更新.
*/
void MysteryLight::Update()
{
	_LocalTime += Time::DeltaTime();
	if (_LocalTime >= _LightTime)
	{
		INSTANCE(HistoryManager)->Evolution();
		_Alpha -= 0.5f * Time::DeltaTime();
		if (_Alpha <= 0.0f)
		{
			_HistoryMenu->SetIsOperation(true);
			SetActive(false, true);
		}
	}

	_LightPlate->transform->SetLocalScale(Vector3::one * fminf(1.0f, _LocalTime) * 15.0f);
	_LightPlate->SetBlendColor(Color(2.0f, 2.0f, 2.0f, _Alpha));
}

void MysteryLight::SetActive(const bool & act, const bool & child)
{
	if (act)
	{
		_Alpha = 1.0f;
		_LocalTime = 0.0f;
		/*Camera* camera = _PlayerCamera->GetComponent<Camera>();
		Vector3 cameraFoward = camera->GetTarget() - _PlayerCamera->transform->GetPosition();
		cameraFoward.Normalize();
		cameraFoward.Scale(0.011f);
		_LightPlate->transform->SetLocalPosition(_PlayerCamera->transform->GetPosition() + cameraFoward);*/
	}
	GameObject::SetActive(act, child);
}