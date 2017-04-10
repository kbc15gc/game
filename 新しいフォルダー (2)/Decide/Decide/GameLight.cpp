#include "GameLight.h"
#include "fbEngine/Light.h"

void GameLight::Awake()
{
	Light* light = AddComponent<Light>();
	GameObjectManager::mainLight = light;
	int num = 4;
	DirectionalLight* Dl[4];
	FOR(num)
		Dl[i] = new DirectionalLight();
	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//奥
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//奥
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//左
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//下

	Dl[0]->SetColor(Color(0.7f, 0.7f, 0.7f, 1.0f));	//奥
	Dl[1]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//右
	Dl[2]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//左
	Dl[3]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//下
	FOR(num)
		light->AddLight(Dl[i]);
}

void GameLight::Update()
{

}
