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
	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//‰œ
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//‰œ
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//¶
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//‰º

	Dl[0]->SetColor(Color(0.5f, 0.5f, 0.5f, 50.0f));	//‰œ
	Dl[1]->SetColor(Color(0.1f, 0.1f, 0.1f, 5.0f));	//‰E
	Dl[2]->SetColor(Color(0.1f, 0.1f, 0.1f, 5.0f));	//¶
	Dl[3]->SetColor(Color(0.1f, 0.1f, 0.1f, 5.0f));	//‰º
	FOR(i, num)
	{
		light->AddLight(Dl[i]);
	}

	ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();
	shadow->SetNear(1.0f);
	shadow->SetFar(100.0f);
}

/**
* ‰Šú‰».
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
