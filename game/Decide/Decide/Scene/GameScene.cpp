#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"
#include"GameObject\Ground\Dungeon.h"
#include"GameObject\Ground\RockCave.h"
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
#include "GameObject\Enemy\EnemySoldier.h"
#include "GameObject\Enemy\BossDrarian.h"

ImageObject* g_depth;

//#define _NKMT_
//#define _NOBO_

namespace
{
	//�{�X
	float BOSS_RADIUS = 35.0f;
	Vector3 BOSS_POS = { -686.0f,61.9f,68.0f };
	//�X
	float MATI_RADIUS = 35.0f;
	Vector3 MATI_POS = { -387.3f,58.0f,-75.8f };
	Vector3 MATI2_POS = { -108.1f ,55.5f ,533.9f };

	SCollisionInfo soundcollisition[]
	{
		#include "Asset\Collisition\ExportSoundCollisition.h"
	};

	Vector3 PlayerScale = { 1.0f,1.0f,1.0f };
}


void GameScene::Start()
{
	INSTANCE(EventManager)->ReSet();

	//�Q�[�����C�g����
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//�v���C���[����
	_Player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 2);


	//�v���C���[�J��������
	GameCamera* playerCamera = INSTANCE(GameObjectManager)->AddNew<PlayerCamera>("PlayerCamera", 8);
	playerCamera->ActiveCamera();

#ifdef _DEBUG

	//�ӂ���J�����̐����B
	GameCamera* thirdPersonCamera = INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);
	//�v���C���[�J�����̎��̃J�����͂ӂ���J�������w��B
	playerCamera->SetNextCamera(thirdPersonCamera);

	//�t���[�J�����̐����B
	GameCamera* freeCamera =INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);
	//�ӂ���J�����̎��̃J�����̓t���[�J�������w��B
	thirdPersonCamera->SetNextCamera(freeCamera);

	//�t���[�J�����̎��̃J�����̓v���C���[�J�������w��B
	freeCamera->SetNextCamera(playerCamera);
#endif

	// ��ԕ��������B
	INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", System::MAX_PRIORITY);

	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 0); //@todo ���̕`��e�X�g�̂��߂ɕ`��D���1����0�ɕύX���Ă���B
	//�_���W��������
	INSTANCE(GameObjectManager)->AddNew<Dungeon>("Dungeon", 1);
	//���A����
	INSTANCE(GameObjectManager)->AddNew<RockCave>("RockCave", 1);
	//�C����.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// �G�l�~�[�}�l�[�W���[�������B
	INSTANCE(EnemyManager)->Start();

	//@todo for debug
	// �e�X�g�B
	// ���X�{�X�쐬�B
	//LastBoss* enemy = INSTANCE(GameObjectManager)->AddNew<LastBoss>("LastBoss", 1);
	//// �p�����[�^�[�ݒ�B
	//vector<BarColor> Color;
	//Color.push_back(BarColor::Blue);
	//Color.push_back(BarColor::Green);
	//Color.push_back(BarColor::Yellow);
	//Color.push_back(BarColor::Red);
	//vector<int> param = vector<int>(static_cast<int>(CharacterParameter::Param::MAX), 10);
	//enemy->SetParamAll(Color, param);

	FOR(i,2)
	{
		//���j�`�b�v
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 2);
		chip->SetChipID((ChipID)i);
	}

	//���j���[
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//���j��
	INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 2);

	INSTANCE(GameObjectManager)->AddNew<AttentionTextOnly>("AttentionTextOnly", 10);

	INSTANCE(GameObjectManager)->AddNew<StatusWindow>("StatusWindow", StatusWindow::WindowBackPriorty);
	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

	INSTANCE(HistoryManager)->Start();

	INSTANCE(ItemManager)->LoadAllItemData();

	INSTANCE(Inventory)->Initialize();

	INSTANCE(GameObjectManager)->AddNew<BackWindowAndAttentionText>("BackWindowAndAttentionText", 10);


#ifdef _NKMT
	INSTANCE(GameObjectManager)->AddNew<TestObject>("TestObject", 9);
#endif // _NKMT

	//�ʏ�BGM
	_WorldBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldBGM->Init("Asset/Sound/Battle_BGM.wav");
	
	//BOSSBGM
	_BossBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("BossBGM", 9);
	_BossBGM->Init("Asset/Sound/boss1.wav");

	//�XBGM
	_MatiBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("MatiBGM", 9);
	_MatiBGM->Init("Asset/Sound/mati1.wav");

	//�X2BGM
	_Mati2BGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Mati2BGM", 9);
	_Mati2BGM->Init("Asset/Sound/mati2.wav");

	//���SBGM
	_DeadBGM = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DeadBGM", 9);
	_DeadBGM->Init("Asset/Sound/dead.wav");
	_DeadBGM->SetVolume(3.0f);

	//�Đ��pBGM
	_GameBGM = _WorldBGM;
	
#ifndef _NOBO_
	_GameBGM->Play(true);
#endif // !_NOBO_
	//�V���h�E�}�b�v�L��.
	_isShadowMap = true;
	//���}�b�v�L��.
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
	//�X�^�[�g�{�^���̉����m�F
	bool back = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	bool start = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((back && start) || KeyBoardInput->isPush(DIK_DELETE))
	{
		//�^�C�g���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("TitleScene",true);
		INSTANCE(Inventory)->deleteList();
		return;
	}

	Collision *coll;
	if(coll = INSTANCE(GameObjectManager)->mainCamera->GetClickCollision(1000.0f, (int)fbCollisionAttributeE::CHARACTER))
	{
		int a = 0;
	}

	// �e�X�g�B
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START)) {
		INSTANCE(Inventory)->ArrangementInventory();
	}
	
	//BGM�ύX������
	{
		if (_Player->GetState() == Player::State::Death)
		{
			ChangeBGM(BGM::DEAD);
		}
		else
		{
			//�e�ꏊ�̃R���W�����ɓ������Ă��邩�B
			for (int i = 0; i < sizeof(soundcollisition) / sizeof(soundcollisition[0]); i++)
			{
				if (IsCollideBoxAABB(soundcollisition[i].pos - soundcollisition[i].scale / 2, soundcollisition[i].pos + soundcollisition[i].scale / 2, _Player->transform->GetPosition() - PlayerScale / 2, _Player->transform->GetPosition() + PlayerScale / 2))
				{
					ChangeBGM(static_cast<BGM>(i));
					break;
				}
			}
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
		case GameScene::BGM::MATI2:
			_GameBGM = _Mati2BGM;
			break;
		case GameScene::BGM::DEAD:
			_GameBGM = _DeadBGM;
			break;
		default:
			break;
		}
		_GameBGM->Play(true);

	}
}


bool GameScene::IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2)
{
	if (vMin1.x < vMax2.x && vMax1.x > vMin2.x
		&& vMin1.y < vMax2.y && vMax1.y > vMin2.y
		&& vMin1.z < vMax2.z && vMax1.z > vMin2.z)
	{
		return true;
	}
	return false;
};