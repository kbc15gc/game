#include"stdafx.h"
#include "GameLight.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"

void GameLight::Awake()
{
	Light* light = AddComponent<Light>();
	INSTANCE(GameObjectManager)->mainLight = light;
	int num = 4;
	DirectionalLight* Dl[4];
	FOR(i, num)
		Dl[i] = new DirectionalLight();
	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//奥
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//奥
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//左
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//下

	Dl[0]->SetColor(Color(0.7f, 0.7f, 0.7f, 1.0f));	//奥
	Dl[1]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//右
	Dl[2]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//左
	Dl[3]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//下
	FOR(i, num)
	{
		light->AddLight(Dl[i]);
	}

	ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();
	shadow->SetNear(1.0f);
	shadow->SetFar(100.0f);
}

/**
* 初期化.
*/
void GameLight::Start()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	INSTANCE(SceneManager)->GetSky()->SetDayAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	INSTANCE(SceneManager)->GetSky()->SetNightAmbientLight(Vector3(0.3f, 0.3f, 0.3f));
}

void GameLight::Update()
{
	ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();

	Vector3 lightPos = INSTANCE(SceneManager)->GetSky()->GetSunPosition();
	lightPos.Normalize();
	lightPos.Scale(30.0f);
	lightPos.Add(_Player->transform->GetPosition());
	shadow->SetLightPosition(lightPos);
	shadow->SetLightTarget(_Player->transform->GetPosition());

}
