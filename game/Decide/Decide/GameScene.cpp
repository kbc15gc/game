#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameCamera.h"
#include "GameShadowCamera.h"

#include "Ground.h"
#include "Sky.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject/HistoryChip/FireChip.h"
#include "GameObject\HistoryChip\TetuChip.h"
#include "GameObject\HistoryChip\AburaChip.h"

#include "GameObject\Village\HistoryMenu.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "GameObject\Village\HistoryMenuSelect.h"

#include "GameObject\Village\Shop.h"
ImageObject* g_depth;

void GameScene::Start()
{
	//ゲームライト生成
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);
	//ゲームカメラ生成
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 8);
	//影カメラ生成
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 8);
	//空生成
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 0);
	//地面生成
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//プレイヤー生成
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// 雑魚エネミープロト生成。
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
	//火の歴史チップ
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);
	//鉄の歴史チップ
	INSTANCE(GameObjectManager)->AddNew<TetuChip>("TetuChip", 1);
	//油の歴史チップ
	INSTANCE(GameObjectManager)->AddNew<AburaChip>("AburaChip", 1);
	//メニュー
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	HistoryBook* book = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 1);
	//メニューセレクト
	INSTANCE(GameObjectManager)->AddNew<HistoryMenuSelect>("HistoryMenuSelect", 9);

	g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(0));
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);

	//歴史で生成されるオブジェクト生成。
	INSTANCE(HistoryManager)->CreateObject();

	//歴史書の親にプレイヤーを設定。
	book->transform->SetParent(player->transform);

	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);

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