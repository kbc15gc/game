#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"

#include "GameObject\MeshObject\MeshObjectChipManager.h"

#include"GameObject\Ground\Dungeon.h"
#include"GameObject\Ground\RockCave.h"
#include"GameObject\Nature\Ocean\Ocean.h"


#include "GameObject\Enemy\Enemy.h"
#include "GameObject\Enemy\BossDrarian.h"

#include "GameObject\History\HistoryManager.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"
#include "GameObject\History\HistoryMenu\HistoryMenu.h"
#include "GameObject\History\Chip.h"
#include "GameObject\Village\VillageName.h"

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

#include "GameObject\Enemy\EnemySoldier.h"
#include "GameObject\Enemy\BossDrarian.h"
#include "GameObject\Enemy\BossGolem.h"
#include "GameObject\Enemy\CodeNameD.h"

#include "fbEngine/_Object/_GameObject/Movie.h"

ImageObject* g_depth;

//#define _NKMT_
//#define _NOBO_

namespace
{
	//ボス
	float BOSS_RADIUS = 35.0f;
	Vector3 BOSS_POS = { -686.0f,61.9f,68.0f };
	//街
	float MATI_RADIUS = 35.0f;

	SCollisionInfo soundcollisition[]
	{
		#include "Asset\Collisition\ExportSoundCollisition.h"
	};

	Vector3 PlayerScale = { 1.0f,1.0f,1.0f };
}


void GameScene::Start()
{
	//最初からならオープニング再生するよー。
	if (IS_CONTINUE == false)
	{
		//オープニング動画。
		auto movie = INSTANCE(GameObjectManager)->AddNew<Movie>("movie", 10);
		movie->Init(L"op.wmv");
		movie->Play();
	}

	INSTANCE(EventManager)->ReSet();

	//ゲームライト生成
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//プレイヤー生成
	_Player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 2);


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

	//メッシュコライダーオブジェクトを生成
	//INSTANCE(GameObjectManager)->AddNew<MeshObjectChipManager>("MeshObjectManager", 1);
	//ダンジョン生成
	//INSTANCE(GameObjectManager)->AddNew<Dungeon>("Dungeon", 1);
	//洞窟生成
	INSTANCE(GameObjectManager)->AddNew<RockCave>("RockCave", 1);
	//海生成.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// エネミーマネージャー初期化。
	INSTANCE(EnemyManager)->Start();

	//@todo for debug
	// テスト。
	// ラスボス作成。
	//LastBoss* enemy = INSTANCE(GameObjectManager)->AddNew<LastBoss>("LastBoss", 1);
	// パラメーター設定。
	vector<BarColor> Color;
	Color.push_back(BarColor::Blue);
	Color.push_back(BarColor::Green);
	Color.push_back(BarColor::Yellow);
	Color.push_back(BarColor::Red);
	vector<int> param = vector<int>(static_cast<int>(CharacterParameter::Param::MAX), 10);
	//enemy->SetParamAll(Color, param);
	
	//メニュー
	_HistoryMenu = INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//歴史書
	_HistoryBook = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 9);

	INSTANCE(GameObjectManager)->AddNew<AttentionTextOnly>("AttentionTextOnly", 10);

	INSTANCE(GameObjectManager)->AddNew<StatusWindow>("StatusWindow", StatusWindow::WindowBackPriorty);
	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

	INSTANCE(HistoryManager)->Start();

	INSTANCE(ItemManager)->LoadAllItemData();

	INSTANCE(Inventory)->Initialize();

	INSTANCE(GameObjectManager)->AddNew<BackWindowAndAttentionText>("BackWindowAndAttentionText", 10);

	_VillageName = INSTANCE(GameObjectManager)->AddNew<VillageName>("VillageName", 10);

#ifdef _NKMT_
	INSTANCE(GameObjectManager)->AddNew<TestObject>("TestObject", 9);
#endif // _NKMT_
	for (int i = 0; i < static_cast<int>(BGM::NUM); i++)
	{
		_SoundBGM[i] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("BGM", 9);
	}
	InitBGM(BGM::WORLD, "Asset/Sound/Battle_BGM.wav", 0.2f);
	InitBGM(BGM::BOSS1, "Asset/Sound/boss1.wav", 0.2f);
	InitBGM(BGM::MATI1, "Asset/Sound/mati1.wav", 0.2f);
	InitBGM(BGM::MATI2, "Asset/Sound/mati2.wav", 0.2f);
	InitBGM(BGM::MATI3, "Asset/Sound/mati3.wav", 0.2f);
	InitBGM(BGM::MAOU, "Asset/Sound/LastDangion2.wav", 1.0f);
	InitBGM(BGM::DEAD, "Asset/Sound/dead.wav", 0.2f);
	//再生用BGM
	_GameBGM = _SoundBGM[static_cast<int>(BGM::WORLD)];
//#ifndef _NOBO_
//	_GameBGM->Play(true);
//#endif // !_NOBO_
	//シャドウマップ有効.
	_isShadowMap = true;
	//環境マップ有効.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable();


	//FOR(i,2)
	//{
	//	//歴史チップ
	//	Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 2);
	//	chip->SetChipID((ChipID)i);
	//}
	//チップを作成
	_NewChip();

	//g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	//g_depth->SetTexture(INSTANCE(SceneManager)->GetBloom().GetLuminanceRT()->texture);
	//g_depth->SetPivot(Vector2(0, 0));
	//g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	//g_depth->SetActive(true);
}

void GameScene::Update()
{
	//@todo for debug
	//デバッグ機能だと思うのでデバッグ専用にしときます。
	//必要な場合は変えてください。
#ifdef _DEBUG
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
	
#endif
	//
	if (KeyBoardInput->isPush(DIK_P))
	{
		INSTANCE(EventManager)->Execute(Event::EventID::EventCameraF, 0);
	}
	//BGM変更したい
	//場所によってBGM変更
	{
		if (_Player->GetState() == Player::State::Death)
		{
			_ChangeBGM(BGM::DEAD);
		}
		else
		{
			int location = -1;
			//各場所のコリジョンに当たっているか。
			int i = 0,size = sizeof(soundcollisition) / sizeof(soundcollisition[0]);
			for (i = 0; i < size; i++)
			{
				if (_IsCollideBoxAABB(soundcollisition[i].pos - soundcollisition[i].scale / 2, soundcollisition[i].pos + soundcollisition[i].scale / 2, _Player->transform->GetPosition() - PlayerScale / 2, _Player->transform->GetPosition() + PlayerScale / 2))
				{
					switch ((BGM)i)
					{
						case BGM::MATI1:
							location = 0;
							_Player->SetRespawnPos(LocationPosition[location]);
							if(!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Begin);
							break;
						case BGM::MATI2:
							location = 1;
							_Player->SetRespawnPos(LocationPosition[location]);
							if (!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Hunting);
							break;
						case BGM::MATI3:
							location = 2;
							_Player->SetRespawnPos(LocationPosition[location]);
							if (!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Prosperity);
							break;
					}
					_ChangeBGM(static_cast<BGM>(i));
					_VillageName->Excute(i);
					break;
				}
			}

			INSTANCE(HistoryManager)->SetNowLocation(location);

			if (i == size - 1)
				_VillageName->Excute(i);
		}
	}
}

void GameScene::_NewChip()
{
	//必要なチップを設置する。
	//所持されていないチップを作成する。
	//火
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Fire))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("FireChip", 2);
		chip->SetChipID(ChipID::Fire);
	}

	//木
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Tree))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("TreeChip", 2);
		chip->SetChipID(ChipID::Tree);
	}

	//狩
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Hunt))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("HuntChip", 2);
		chip->SetChipID(ChipID::Hunt);
	}

	//農
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Agriculture))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("AgricultureChip", 2);
		chip->SetChipID(ChipID::Agriculture);
	}
	//鉄
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Iron))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("IronChip", 2);
		chip->SetChipID(ChipID::Iron);
	}
	//油
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Oil))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("OilChip", 2);
		chip->SetChipID(ChipID::Oil);
	}
	//@todo for debug
	//テスト用
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Medicine))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("MedicineChip", 2);
		chip->SetChipID(ChipID::Medicine);
	}
}

void GameScene::_ChangeBGM(BGM bgm)
{
	if (_BGM != bgm)
	{
		//BGM変更
		_BGM = bgm;
		//現在のBMGストップ
		_GameBGM->Stop();
		//サウンドを設定。
		_GameBGM = _SoundBGM[static_cast<int>(_BGM)];
		//再生
		_GameBGM->Play(true);

	}
}

void GameScene::InitBGM(BGM bgm, char* name, float volume)
{
	_SoundBGM[static_cast<int>(bgm)]->Init(name);
	_SoundBGM[static_cast<int>(bgm)]->SetVolume(volume);
}


bool GameScene::_IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2)
{
	if (vMin1.x < vMax2.x && vMax1.x > vMin2.x
		&& vMin1.y < vMax2.y && vMax1.y > vMin2.y
		&& vMin1.z < vMax2.z && vMax1.z > vMin2.z)
	{
		return true;
	}
	return false;
};