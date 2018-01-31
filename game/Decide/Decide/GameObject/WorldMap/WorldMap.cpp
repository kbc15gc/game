#include "stdafx.h"
#include "WorldMap.h"
#include "fbEngine\_Nature\Sky.h"
#include "GameObject\Nature\Ocean\Ocean.h"
#include "GameLight.h"
#include "GameObject\Ground\Ground.h"
#include "Scene\GameScene.h"
#include "GameObject\SplitSpace.h"


void MapLight::Awake()
{
	_Light = AddComponent<Light>();

	int num = 4;
	DirectionalLight* Dl[4];
	FOR(i, num)
	{
		Dl[i] = new DirectionalLight();
	}

	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//奥
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//奥
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//左
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//下

	Dl[0]->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));	//奥
	Dl[1]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//右
	Dl[2]->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));	//左
	Dl[3]->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));	//下

	FOR(i, num)
	{
		_Light->AddLight(Dl[i]);
	}


	//ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();
	//shadow->SetNear(1.0f);
	//shadow->SetFar(100.0f);
}

/**
* 初期化.
*/
void MapLight::Start()
{
	//INSTANCE(SceneManager)->GetSky()->SetDayAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	//INSTANCE(SceneManager)->GetSky()->SetNightAmbientLight(Vector3(0.3f, 0.3f, 0.3f));
}

void MapLight::Update()
{
}

/**
* 初期化.
*/
void WorldMap::Awake() {
	//ふかんカメラの生成。
	_camera = INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);
	_mapLight = INSTANCE(GameObjectManager)->AddNew<MapLight>("WorldMapLight",7);

	// ゲーム開始時は非アクティブ。
	this->SetActive(false);
	_isChangeFrame = false;

}

/**
* 更新.
*/
void WorldMap::PreUpdate() {
	// フラグ初期化。
	_isChangeFrame = false;
}

/**
* 更新.
*/
void WorldMap::Update() {
	if (VPadInput->IsPush(fbEngine::VPad::ButtonSelect) || VPadInput->IsPush(fbEngine::VPad::ButtonB)) {
		Close();
	}
}

/**
* マップを開く。
*/
void WorldMap::Open() {
	//if (!_isChangeFrame) {
		// 同じフレームで先にClose関数が呼ばれていればこの処理は無視。

		// マップを開く処理が呼ばれたのでアクティブ化。
		this->SetActive(true);
		// 現在のカメラを保存。
		_camera->SetNextCamera(static_cast<GameCamera*>(INSTANCE(GameObjectManager)->mainCamera->gameObject));
		// 俯瞰カメラに切り替え。
		_camera->ActiveCamera();
		INSTANCE(SceneManager)->GetSky()->SetDisable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(false);
		//INSTANCE(SceneManager)->GetBloom().SetEnable(false);
		//INSTANCE(SceneManager)->GetDepthofField().SetEnable(false);
		_saveLight = INSTANCE(GameObjectManager)->mainLight;
		//INSTANCE(GameObjectManager)->mainLight = _mapLight->GetLight();
		INSTANCE(GameObjectManager)->FindObject("Ground")->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);
		_split = static_cast<GameScene*>(INSTANCE(SceneManager)->GetNowScene())->GetNowSplitSpace();
		if (_split) {
			_split->TargetLost();
			_split->SetActive(false);
		}

		_maouSiro = INSTANCE(GameObjectManager)->FindObject("MaouSiro.X");
		if (_maouSiro)
		{
			_maouSiro->SetActive(false);
		}

		_isChangeFrame = true;
	//}
}

// マップを閉じる。
void WorldMap::Close() {
	if (!_isChangeFrame) {
		// 同じフレームで先にOpen関数が呼ばれていればこの処理は無視。

		this->SetActive(false);
		if (_camera->GetNextCamera()) {
			// 元のカメラに戻す。
			_camera->GetNextCamera()->ActiveCamera();
		}

		INSTANCE(SceneManager)->GetSky()->SetEnable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(true);
		INSTANCE(GameObjectManager)->FindObject("Ground")->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

		//INSTANCE(GameObjectManager)->mainLight = _saveLight;

		if (_split) {
			_split->SetActive(true);
		}
		if (_maouSiro)
		{
			_maouSiro->SetActive(true);
		}

		// ワールドマップモードを終了したことをイベントマネージャーに通知。
		INSTANCE(EventManager)->NotifyEndEvent();

		_isChangeFrame = true;
	}
}
