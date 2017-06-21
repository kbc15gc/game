#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameCamera.h"

#include "Ground.h"
#include "Ocean.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"

#include "GameObject\Village\HistoryMenu.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "GameObject\Village\HistoryMenuSelect.h"

#include "GameObject\Village\Shop.h"
#include "GameObject\Village\ItemManager.h"
#include "GameObject\HistoryChip\Chips.h"

#include "ThirdPersonCamera.h"
#include "FreeCamera.h"

ImageObject* g_depth;

void GameScene::Start()
{
	//ゲームライト生成
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);
	//ゲームカメラ生成
	GameCamera* camera = INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 8);
	//地面生成
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//海生成.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);
	//プレイヤー生成
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// 雑魚エネミープロト生成。
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 9);

	FOR(i,ChipID::NUM)
	{
		//歴史チップ
		Chips* chip = INSTANCE(GameObjectManager)->AddNew<Chips>("Chip", 1);
		chip->SetChipID((ChipID)i);
	}

	//メニュー
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	HistoryBook* book = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 1);
	//歴史書の親にプレイヤーを設定。
	book->transform->SetParent(player->transform);

	//メニューセレクト
	INSTANCE(GameObjectManager)->AddNew<HistoryMenuSelect>("HistoryMenuSelect", 9);

	//歴史で生成されるオブジェクト生成。
	INSTANCE(HistoryManager)->CreateObject();

	//ふかんカメラの生成。
	INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);

	//フリーカメラの生成。
	INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);



	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	INSTANCE(ItemManager)->LoadItemData();
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	shop->OpenShop(0);

	_WorldSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldSE->InitStreaming("Asset/Sound/Battle_BGM.wav");
	_WorldSE->Play(true);

	//シャドウマップ有効.
	_isShadowMap = true;
	//環境マップ有効.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable(camera->GetComponent<Camera>(), light->GetComponent<Light>());

	g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetDepthofField().GetDepthRenderTarget()->texture);
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);

}

void GameScene::Update()
{
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//フェードイン開始
		StartFade(true);
	}
	//フェードイン完了
	if (_FadeState == fbScene::FadeStateE::INEND)
	{
		//タイトルシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("TitleScene");
		return;
	}
}