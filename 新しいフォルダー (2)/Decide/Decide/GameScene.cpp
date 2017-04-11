#include "GameScene.h"
#include "GameCamera.h"
#include "GameLight.h"
#include "GameShadowCamera.h"
#include "Player.h"
#include "Ground.h"
#include "Sky.h"
#include "fbEngine/ImageObject.h"
#include "fbEngine/TextObject.h"
#include "fbEngine/Sprite.h"
#include "fbEngine/SoundSource.h"

void GameScene::Start()
{
	GameObjectManager::AddNew<GameCamera>("GameCamera", 2);
	GameObjectManager::AddNew<GameLight>("GameLight", 0);
	GameObjectManager::AddNew<GameShadowCamera>("GameShadowCamera", 0);
	//プレイヤー生成
	GameObjectManager::AddNew<Player>("Player", 1);

	//地面生成
	GameObjectManager::AddNew<Ground>("Ground", 1);
	//空生成
	GameObjectManager::AddNew<Sky>("Sky", 1);
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