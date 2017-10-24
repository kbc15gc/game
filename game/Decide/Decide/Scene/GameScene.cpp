#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"
#include"GameObject\Ground\Dungeon.h"
#include"GameObject\Nature\Ocean\Ocean.h"


#include "GameObject\Enemy\Enemy.h"
#include "GameObject\Enemy\BossDrarian.h"

#include "GameObject\History\HistoryManager.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"
#include "GameObject\History\HistoryMenu\HistoryMenu.h"
#include "GameObject\History\Chip.h"

#include "GameObject\Village\EventManager.h"

#include "GameObject\ItemManager\ItemManager.h"
#include "GameObject\Inventory\Inventory.h"

#include "GameObject\Camera\PlayerCamera.h"
#include "GameObject\Camera\ThirdPersonCamera.h"
#include "GameObject\Camera\FreeCamera.h"
#include "GameObject\Enemy\EnemyManager.h"
#include "GameObject\SplitSpace.h"

#include "GameObject\Village\Shop\Shop.h"

#include"GameObject\GameManager.h"
#include"GameObject\StatusWindow\StatusWindow.h"
#include "GameObject\Enemy\LastBoss.h"

#include"_Debug\TestObject.h"

#include "GameObject\TextImage\BackWindowAndAttentionText.h"
#include "GameObject\TextImage\AttentionTextOnly.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"

ImageObject* g_depth;

//#define _NKMT_

namespace
{
	//ボス
	float BOSS_RADIUS = 35.0f;
	Vector3 BOSS_POS = { -686.0f,61.9f,68.0f };
	//街
	float MATI_RADIUS = 35.0f;
	Vector3 MATI_POS = { -387.3f,58.0f,-75.8f };
}


void GameScene::Start()
{
	INSTANCE(EventManager)->ReSet();

	//ゲームライト生成
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//プレイヤー生成
	_Player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);


	//プレイヤーカメラ生成
	GameCamera* playerCamera = INSTANCE(GameObjectManager)->AddNew<PlayerCamera>("PlayerCamera", 8);
	playerCamera->ActiveCamera();

#ifdef _DEBUG

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
#endif

	// 空間分割生成。
	INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", System::MAX_PRIORITY);

	//地面生成
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 0); //@todo 草の描画テストのために描画優先を1から0に変更している。
	//ダンジョン生成
	INSTANCE(GameObjectManager)->AddNew<Dungeon>("Dungeon", 1);
	//海生成.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// エネミーマネージャー初期化。
	INSTANCE(EnemyManager)->Start();

	FOR(i,2)
	{
		//歴史チップ
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 1);
		chip->SetChipID((ChipID)i);
	}

	//メニュー
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 2);

	INSTANCE(GameObjectManager)->AddNew<AttentionTextOnly>("AttentionTextOnly", 10);

	INSTANCE(GameObjectManager)->AddNew<StatusWindow>("StatusWindow", StatusWindow::WindowBackPriorty);
	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

	INSTANCE(HistoryManager)->Start();

	INSTANCE(ItemManager)->LoadAllItemData();

	INSTANCE(Inventory)->Initialize();

	INSTANCE(GameObjectManager)->AddNew<BackWindowAndAttentionText>("BackWindowAndAttentionText", 10);

	DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 8);
	item->Load();

#ifdef _NKMT
	INSTANCE(GameObjectManager)->AddNew<TestObject>("TestObject", 9);
#endif // _NKMT

	//通常BGM
	_WorldBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldBGM->Init("Asset/Sound/Battle_BGM.wav");
	
	//BOSSBGM
	_BossBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("BossBGM", 9);
	_BossBGM->Init("Asset/Sound/boss1.wav");

	//街BGM
	_MatiBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("MatiBGM", 9);
	_MatiBGM->Init("Asset/Sound/mati1.wav");

	//再生用BGM
	_GameBGM = _WorldBGM;
	_GameBGM->Play(true);

	//シャドウマップ有効.
	_isShadowMap = true;
	//環境マップ有効.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable();


	/*g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(0));
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(true);*/
}

void GameScene::Update()
{
	//スタートボタンの押下確認
	bool back = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	bool start = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//エンターキー
	if ((back && start) || KeyBoardInput->isPush(DIK_DELETE))
	{
		//タイトルシーンへ移行
		INSTANCE(SceneManager)->ChangeScene("TitleScene",true);
		INSTANCE(Inventory)->deleteList();
		return;
	}

	Collision *coll;
	if(coll = INSTANCE(GameObjectManager)->mainCamera->GetClickCollision(1000.0f, (int)fbCollisionAttributeE::CHARACTER))
	{
		int a = 0;
	}

	// テスト。
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START)) {
		INSTANCE(Inventory)->ArrangementInventory();
	}
	
	//BGM変更したい
	{
		//ボス
		Vector3 boss_dir = BOSS_POS - _Player->transform->GetPosition();
		float boss_len = boss_dir.Length();
		//街
		Vector3 mati_dir = MATI_POS - _Player->transform->GetPosition();
		float mati_len = mati_dir.Length();
		//街
		if (mati_len < MATI_RADIUS)
		{
			ChangeBGM(BGM::MATI1);
		}
		//ボス
		if (boss_len < BOSS_RADIUS)
		{
			ChangeBGM(BGM::BOSS1);
		}
		//ワールド
		if(boss_len > BOSS_RADIUS && mati_len > MATI_RADIUS)
		{
			ChangeBGM(BGM::WORLD);
		}
	}

}

void GameScene::ChangeBGM(BGM bgm)
{
	if (_BGM != bgm)
	{
		_BGM = bgm;
		_GameBGM->Stop();
		switch (bgm)
		{
		case GameScene::BGM::WORLD:
			_GameBGM = _WorldBGM;
			break;
		case GameScene::BGM::BOSS1:
			_GameBGM = _BossBGM;
			break;
		case GameScene::BGM::MATI1:
			_GameBGM = _MatiBGM;
			break;
		default:
			break;
		}
		_GameBGM->Play(true);

	}
}
