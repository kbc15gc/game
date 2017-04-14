#include "GameScene.h"
#include "GameCamera.h"
#include "GameLight.h"
#include "GameShadowCamera.h"
#include "GameObject/Player/Player.h"
#include "Ground.h"
#include "Sky.h"
#include "fbEngine/ImageObject.h"
#include "fbEngine/TextObject.h"
#include "fbEngine/Sprite.h"
#include "fbEngine/SoundSource.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject/HistoryChip/FireChip.h"

void GameScene::Start()
{
	//プレイヤー生成
	GameObjectManager::AddNew<Player>("Player", 1);
	//ゲームカメラ生成
	GameObjectManager::AddNew<GameCamera>("GameCamera", 2);
	//ゲームライト生成
	GameObjectManager::AddNew<GameLight>("GameLight", 0);
	//影カメラ生成
	GameObjectManager::AddNew<GameShadowCamera>("GameShadowCamera", 2);
	//地面生成
	GameObjectManager::AddNew<Ground>("Ground", 1);
	//空生成
	GameObjectManager::AddNew<Sky>("Sky", 1);
	// 雑魚エネミープロト生成。
	GameObjectManager::AddNew<Enemy>("EnemyProt",1);
	//火の歴史チップ
	GameObjectManager::AddNew<FireChip>("FireChip", 1);


	//ImageObject* showDepth = GameObjectManager::AddNew<ImageObject>("ShowDepth", 4);
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