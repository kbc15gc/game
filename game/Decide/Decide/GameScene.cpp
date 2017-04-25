#include "GameScene.h"
#include "GameCamera.h"
#include "GameLight.h"
#include "GameShadowCamera.h"
#include "GameObject/Player/Player.h"
#include "Ground.h"
#include "Sky.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject/HistoryChip/FireChip.h"

void GameScene::Start()
{
	//プレイヤー生成
	INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	//ゲームカメラ生成
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 2);
	//ゲームライト生成
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 0);
	//影カメラ生成
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 2);
	//地面生成
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//空生成
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 1);
	// 雑魚エネミープロト生成。
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt",1);
	//火の歴史チップ
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);


	//ImageObject* showDepth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShowDepth", 4);
	//showDepth->SetTexture(INSTANCE(RenderTargetManager)->GetRTTextureFromList(RTIdxE::SHADOWDEPTH));
	//showDepth->SetPivot(Vector2(0.0f, 0.0f));

}

void GameScene::Update()
{
	//スタートボタンの押下確認
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//チェンジシーンフラグをtrue
		_ChangeScene = true;
		//フェード開始
		SetFade(true);
	}
	if (_ChangeScene &&	//エンターキーが押された
		!_IsFade)		//フェード終了
	{
		//タイトルシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("TitleScene");
		//シーンチェンジ完了
		_ChangeScene = false;
		return;
	}
}