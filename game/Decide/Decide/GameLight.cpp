#include"stdafx.h"
#include "GameLight.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"

void GameLight::Awake()
{
	Light* light = AddComponent<Light>();
	INSTANCE(GameObjectManager)->mainLight = light;
	int num = 4;
	DirectionalLight* Dl[4];
	FOR(i,num)
		Dl[i] = new DirectionalLight();
	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//‰œ
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//‰œ
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//¶
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//‰º

	Dl[0]->SetColor(Color(0.7f, 0.7f, 0.7f, 1.0f));	//‰œ
	Dl[1]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//‰E
	Dl[2]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//¶
	Dl[3]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//‰º
	FOR(i,num)
		light->AddLight(Dl[i]);
}

void GameLight::Update()
{
	static float angle = 0;
	Light* light = GetComponent<Light>();
	light->GetLight()[0]->SetEulerAngles(Vector3(45, angle, 180));

	angle += 1.0f;
}
