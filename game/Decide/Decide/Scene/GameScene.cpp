#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"
#include"GameObject\Nature\Ocean\Ocean.h"

#include "GameObject/Player/Player.h"
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

#include "BuffDebuffICon.h"
ImageObject* g_depth;
void DebugNPC();

void GameScene::Start()
{
	INSTANCE(EventManager)->ReSet();

	//ゲームライト生成
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//バフデバフアイコンを表示するクラス。
	INSTANCE(GameObjectManager)->AddNew<BuffDebuffICon>("BuffDebuffICon", 9);

	//プレイヤー生成
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);


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
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
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
	INSTANCE(GameObjectManager)->AddNew<StatusWindow>("StatusWindow", StatusWindow::WindowBackPriorty);
	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

	INSTANCE(HistoryManager)->Start();

	INSTANCE(ItemManager)->LoadAllItemData();
	INSTANCE(Inventory)->Initialize();


	_WorldSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldSE->InitStreaming("Asset/Sound/Battle_BGM.wav");
	_WorldSE->Play(true);

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
#ifdef _DEBUG
	DebugNPC();
#endif // _DEBUG
}

#ifdef _DEBUG
#include "GameObject\Village\NPC.h"

void DebugNPC()
{
	//デバッグ用にＮＰＣ追加。
	NPC* npc = INSTANCE(GameObjectManager)->AddNew<NPC>("NPC", 2);
	npc->LoadModel("villager1.X");
	npc->SetMesseage(12, true);
	npc->transform->SetLocalPosition(Vector3(-148.0f, 68.5f, -34.0f));
}
#endif // _DEBUG
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
}