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

#include "GameObject\Village\HistoryMenu.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "GameObject\Village\HistoryMenuSelect.h"

ImageObject* depth;

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
	INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// 雑魚エネミープロト生成。
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
	//火の歴史チップ
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);
	//メニュー
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 1);
	//メニューセレクト
	INSTANCE(GameObjectManager)->AddNew<HistoryMenuSelect>("HistoryMenuSelect", 9);

	INSTANCE(HistoryManager)->CreateObject();

	//depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	//depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(1));
	/*ImageObject* depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	depth->SetTexture(INSTANCE(RenderTargetManager)->GetRTTextureFromList(RTIdxE::SHADOWDEPTH));
	depth->SetPivot(Vector2(0, 0));
	depth->SetActive(false);*/
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