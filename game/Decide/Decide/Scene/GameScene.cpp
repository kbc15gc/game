#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"
#include "Ocean.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject\Enemy\BossDrarian.h"

#include "GameObject\History\HistoryManager.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"
#include "GameObject\History\HistoryMenu\HistoryMenu.h"
#include "GameObject\History\Chip.h"

#include "GameObject\Village\Shop.h"
#include "GameObject\Village\ItemManager.h"

#include "GameObject\Camera\PlayerCamera.h"
#include "GameObject\Camera\ThirdPersonCamera.h"
#include "GameObject\Camera\FreeCamera.h"
#include "GameObject\Enemy\EnemyManager.h"
#include "GameObject\SplitSpace.h"

ImageObject* g_depth;

void GameScene::Start()
{
	//ゲームライト生成
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//プレイヤー生成
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);


	//プレイヤーカメラ生成
	GameCamera* playerCamera = INSTANCE(GameObjectManager)->AddNew<PlayerCamera>("PlayerCamera", 8);
	playerCamera->ActiveCamera();

	//ふかんカメラの生成。
	GameCamera* thirdPersonCamera = INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);
	//プレイヤーカメラの次のカメラはふかんカメラを指定。
	playerCamera->SetNextCamera(thirdPersonCamera);

	//フリーカメラの生成。
	GameCamera* freeCamera =INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);
	//ふかんカメラの次のカメラはフリーカメラを指定。
	thirdPersonCamera->SetNextCamera(freeCamera);

	//フリーカメラの次のカメラはプレイヤーカメラを指定。
	freeCamera->SetNextCamera(playerCamera);

	// 空間分割生成。
	INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", System::MAX_PRIORITY);

	//地面生成
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//海生成.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// エネミーマネージャー初期化。
	INSTANCE(EnemyManager)->Start();

	// ボス生成。
	INSTANCE(GameObjectManager)->AddNew<BossDrarian>("Drarian", 1);
	
	FOR(i,ChipID::ChipNum)
	{
		//歴史チップ
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 1);
		chip->SetChipID((ChipID)i);
	}

	//メニュー
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 2);

	INSTANCE(HistoryManager)->Start();
	//歴史で生成されるオブジェクト生成。
	INSTANCE(HistoryManager)->CreateObject();

	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	INSTANCE(ItemManager)->LoadAllData();
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	shop->OpenShop(0);

	_WorldSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldSE->InitStreaming("Asset/Sound/Battle_BGM.wav");
	_WorldSE->Play(true);

	//シャドウマップ有効.
	_isShadowMap = true;
	//環境マップ有効.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable(playerCamera->GetComponent<Camera>(), light->GetComponent<Light>());


	/*g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetDepthofField().GetDepthRenderTarget()->texture);
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);*/

}

void GameScene::Update()
{
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//タイトルシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("TitleScene",true);
		return;
	}

	Collision *coll;
	if(coll = INSTANCE(GameObjectManager)->mainCamera->GetClickCollision(1000.0f, (int)fbCollisionAttributeE::CHARACTER))
	{
		int a = 0;
	}
}