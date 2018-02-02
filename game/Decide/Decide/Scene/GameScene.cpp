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


#include"_Debug\TestObject.h"

#include "GameObject\TextImage\BackWindowAndAttentionText.h"
#include "GameObject\TextImage\AttentionTextOnly.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"

#include "GameObject\Enemy\EnemySoldier.h"
#include "GameObject\Enemy\BossDrarian.h"
#include "GameObject\Enemy\BossGolem.h"
#include "GameObject\Enemy\CodeNameD.h"

#include "fbEngine/_Object/_GameObject/Movie.h"

#include "GameObject\Enemy\LastBoss.h"

#include "GameObject\SplitSpace.h"
#include "fbEngine\_Nature\Sky.h"

#include "GameObject\WorldMap\WorldMap.h"


ImageObject* g_depth;

//#define _NKMT_
//#define _NOBO_

namespace
{
	SCollisionInfo soundcollisition[]
	{
		#include "Asset\Collisition\ExportSoundCollisition.h"
	};

	Vector3 PlayerScale = { 1.0f,1.0f,1.0f };

}


void GameScene::Start()
{
	//�ŏ�����Ȃ�I�[�v�j���O�Đ������[�B
	if (IS_CONTINUE == false)
	{
		//�I�[�v�j���O����B
		auto movie = INSTANCE(GameObjectManager)->AddNew<Movie>("movie", 10);
		movie->LoadVideo(L"op.wmv");
		movie->Play();
	}

	INSTANCE(EventManager)->ReSet();

	//�Q�[�����C�g����
	_GameLight = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);
	_GameLight->SetIsPointLight(false);

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
	GameCamera* freeCamera = INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);
	//�ӂ���J�����̎��̃J�����̓t���[�J�������w��B
	thirdPersonCamera->SetNextCamera(freeCamera);

	//�t���[�J�����̎��̃J�����̓v���C���[�J�������w��B
	freeCamera->SetNextCamera(playerCamera);
#endif


	// ��ԕ��������B
	_splitWorld = INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", System::MAX_PRIORITY);

	// ������p�̋�ԕ��������B
	_splitMaouzyou = INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace_MaouSiro", System::MAX_PRIORITY);

	//�n�ʐ���
	_Ground = INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 0); //@todo ���̕`��e�X�g�̂��߂ɕ`��D���1����0�ɕύX���Ă���B

	//���b�V���R���C�_�[�I�u�W�F�N�g�𐶐�
	//INSTANCE(GameObjectManager)->AddNew<MeshObjectChipManager>("MeshObjectManager", 1);
	//�_���W��������
	//INSTANCE(GameObjectManager)->AddNew<Dungeon>("Dungeon", 1);
	//���A����
	INSTANCE(GameObjectManager)->AddNew<RockCave>("RockCave", 1);
	//�C����.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// �G�l�~�[�}�l�[�W���[�������B
	INSTANCE(EnemyManager)->Start();

	////@todo for debug
	//// �e�X�g�B
	//// ���X�{�X�쐬�B
	//LastBoss* _LastBoss = INSTANCE(GameObjectManager)->AddNew<LastBoss>("LastBoss", 1);
	//// �p�����[�^�[�ݒ�B
	//vector<BarColor> Color;
	//Color.push_back(BarColor::Blue);
	//Color.push_back(BarColor::Green);
	//Color.push_back(BarColor::Yellow);
	//Color.push_back(BarColor::Red);
	//vector<int> param = vector<int>(static_cast<int>(CharacterParameter::Param::MAX), 0);
	//param[CharacterParameter::Param::HP] = 30000;
	//param[CharacterParameter::Param::ATK] = 2000;
	//param[CharacterParameter::Param::MAT] = 2000;
	//param[CharacterParameter::Param::DEF] = 250;
	//param[CharacterParameter::Param::MDE] = 250;
	//param[CharacterParameter::Param::DEX] = 20;
	//param[CharacterParameter::Param::CRT] = 10;

	//_LastBoss->SetParamAll(Color, param);

	//���j���[
	_HistoryMenu = INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//���j��
	_HistoryBook = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 9);

	// ���[���h�}�b�v�B
	_worldMap = INSTANCE(GameObjectManager)->AddNew<WorldMap>("WorldMap", StatusWindow::WindowBackPriorty);

	INSTANCE(GameObjectManager)->AddNew<AttentionTextOnly>("AttentionTextOnly", 10);

	INSTANCE(HistoryManager)->Start();

	INSTANCE(ItemManager)->LoadAllItemData();

	INSTANCE(Inventory)->Initialize();

	INSTANCE(GameObjectManager)->AddNew<StatusWindow>("StatusWindow", StatusWindow::WindowBackPriorty);
	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

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
	InitBGM(BGM::BOSS2, "Asset/Sound/boss1.wav", 0.2f);
	InitBGM(BGM::BOSS3, "Asset/Sound/boss1.wav", 0.2f);
	InitBGM(BGM::MATI1, "Asset/Sound/mati1.wav", 0.2f);
	InitBGM(BGM::MATI2, "Asset/Sound/mati2.wav", 0.2f);
	InitBGM(BGM::MATI3, "Asset/Sound/mati3.wav", 0.2f);
	InitBGM(BGM::MAOU1, "Asset/Sound/LastDangion1.wav", 0.5f);
	InitBGM(BGM::MAOU2, "Asset/Sound/LastDangion2.wav", 0.6f);
	InitBGM(BGM::MAOU3, "Asset/Sound/LastDangion3.wav", 0.8f);
	InitBGM(BGM::DEAD, "Asset/Sound/dead.wav", 0.2f);
	InitBGM(BGM::BOSS, "Asset/Sound/LastBattle1.wav", 0.5f);
	InitBGM(BGM::LASTBOSS, "Asset/Sound/LastBattle2.wav", 1.0f);

	//�Đ��pBGM
	_GameBGM = _SoundBGM[static_cast<int>(BGM::WORLD)];
	_BGM = BGM::NONE;
	//#ifndef _NOBO_
	//	_GameBGM->Play(true);
	//#endif // !_NOBO_
		//�V���h�E�}�b�v�L��.
	_isShadowMap = true;
	//���}�b�v�L��.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable();


	//FOR(i,2)
	//{
	//	//���j�`�b�v
	//	Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 2);
	//	chip->SetChipID((ChipID)i);
	//}
	//�`�b�v���쐬
	_NewChip();

	//�G���f�B���O�t���O
	_IsEnding = false;

	/*g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetNormalRT()->texture);
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(true);*/

	_isFirstFrame = true;	// ��Ɨp�B�����R�[�h�B

	_targetMoonColor = _defaultColor = INSTANCE(SceneManager)->GetSky()->GetMoon()->GetBlendColor();

	// ��ԕ����ōŏ��ɂǂ�����g�p���邩�ɕK�v�B
	{
		_isMaouzyou = true;
		int location = -1;
		//�e�ꏊ�̃R���W�����ɓ������Ă��邩�B
		int i = 0, size = sizeof(soundcollisition) / sizeof(soundcollisition[0]);
		for (i = 0; i < size; i++)
		{
			if (_IsCollideBoxAABB(soundcollisition[i].pos - soundcollisition[i].scale / 2, soundcollisition[i].pos + soundcollisition[i].scale / 2, _Player->transform->GetPosition() - PlayerScale / 2, _Player->transform->GetPosition() + PlayerScale / 2))
			{
				switch ((BGM)i)
				{
				case BGM::MAOU1:
					_isMaouzyou = false;
					break;
				case BGM::MAOU2:
					_isMaouzyou = false;
					break;
				case BGM::MAOU3:
					_isMaouzyou = false;
					break;
				}
				break;
			}
		}
	}

	if (!_isMaouzyou) {
		// �ŏ����疂����ɂ���̂ŕ�ԂȂ��Ŗ�ɂ���B
		INSTANCE(SceneManager)->GetSky()->SetSunMode(Sky::SunMode::Stop, Sky::SunMode::Stop, 225.0f);
		_GameLight->SetIsPointLight(true);
		_Ground->SetIsDL(false);
	}

}

void GameScene::Update()
{
	////�G���f�B���O�ցB
	//if (_LastBoss->GetDeathFlag())
	//{
	//	INSTANCE(SceneManager)->ChangeScene("EndingScene", true);
	//}

	_BefBGM = _BGM;

	//@todo for debug
	//�f�o�b�O�@�\���Ǝv���̂Ńf�o�b�O��p�ɂ��Ƃ��܂��B
	//�K�v�ȏꍇ�͕ς��Ă��������B
#ifdef _DEBUG
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
	
#endif
	//BGM�ύX������
	//�ꏊ�ɂ����BGM�ύX
	{
		if (_Player->GetState() == Player::State::Death)
		{
			_ChangeBGM(BGM::DEAD);
		}
		else
		{
			bool isHit = false;
			int location = -1;
			//�e�ꏊ�̃R���W�����ɓ������Ă��邩�B
			int i = 0,size = sizeof(soundcollisition) / sizeof(soundcollisition[0]);
			for (i = 0; i < size; i++)
			{
				if (_IsCollideBoxAABB(soundcollisition[i].pos - soundcollisition[i].scale / 2, soundcollisition[i].pos + soundcollisition[i].scale / 2, _Player->transform->GetPosition() - PlayerScale / 2, _Player->transform->GetPosition() + PlayerScale / 2))
				{
					isHit = true;

					switch ((BGM)i)
					{
						case BGM::MATI1:
							location = 0;
							_Player->SetRespawnPos(LocationPosition[location],Quaternion());
							if(!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Begin);
							break;
						case BGM::MATI2:
							location = 1;
							_Player->SetRespawnPos(LocationPosition[location], Quaternion());
							if (!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Hunting);
							break;
						case BGM::MATI3:
							location = 2;
							_Player->SetRespawnPos(LocationPosition[location], Quaternion());
							if (!_HistoryBook->GetActive())
								_HistoryMenu->SetLocationCode(LocationCodeE::Prosperity);
							break;
						case BGM::MAOU1:
							_Player->SetRespawnPos(AllLocationPosition[static_cast<int>(LocationCodeAll::Kuni)],Quaternion(/*0.0f, 0.0f, 0.0f, 1.0f*/-0.0f, -1.0f, -0.0f, 0.0f));
							break;
						case BGM::MAOU2:
							break;
						case BGM::MAOU3:
							_Player->SetRespawnPos(AllLocationPosition[static_cast<int>(LocationCodeAll::Kuni)],Quaternion(/*0.0f, 0.0f, 0.0f, 1.0f*/-0.0f, -1.0f, -0.0f, 0.0f));
							break;
					}
					_ChangeBGM(static_cast<BGM>(i));
					_VillageName->Excute(i);
					break;
				}
			}

			if (!isHit) {
				// �ǂ�BGM�R���W�����ɂ��������Ă��Ȃ��̂Ńt�B�[���hBGM���Đ��B
				_ChangeBGM(BGM::WORLD);
			}

			INSTANCE(HistoryManager)->SetNowLocation(location);

			if (i == size - 1)
				_VillageName->Excute(i);

			_isFirstFrame = false;

		}
	}

	_MoveMoonColor();
}

void GameScene::_NewChip()
{
	//�K�v�ȃ`�b�v��ݒu����B
	//��������Ă��Ȃ��`�b�v���쐬����B
	//��
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Fire))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("FireChip", 2);
		chip->SetChipID(ChipID::Fire);
		chip->SetGetTime(0.0f);
	}

	//��
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Tree))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("TreeChip", 2);
		chip->SetChipID(ChipID::Tree);

	}

	//��
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Copper))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("CopperChip", 2);
		chip->SetChipID(ChipID::Copper);
		chip->SetGetTime(0.0f);
	}

	//�_
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Agriculture))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("AgricultureChip", 2);
		chip->SetChipID(ChipID::Agriculture);
		chip->SetGetTime(0.0f);
	}
	//�S
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Iron))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("IronChip", 2);
		chip->SetChipID(ChipID::Iron);
		chip->SetGetTime(0.0f);
	}
	//��
	if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Oil))
	{
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("OilChip", 2);
		chip->SetChipID(ChipID::Oil);
		chip->SetGetTime(0.0f);
	}
}

void GameScene::_ChangeBGM(BGM bgm)
{
	if (bgm < BGM::BOSS) {
		if (!_isFirstFrame) {
			// ��ԕ����؂�ւ��B
			if (!_isMaouzyou) {
				if (bgm == BGM::MAOU1 || bgm == BGM::MAOU2 || bgm == BGM::MAOU3) {
					// ������ɐN���B

					_splitWorld->TargetLost();
					_splitWorld->SetActive(false);
					_splitMaouzyou->TargetLost();
					_splitMaouzyou->SetActive(true);
					_nowSplitSpace = _splitMaouzyou;
					_isMaouzyou = true;
				}
			}
			else {
				if (bgm != BGM::DEAD && bgm != BGM::MAOU1 && bgm != BGM::MAOU2 && bgm != BGM::MAOU3) {
					// �����邩��ł��B

					if (_BefBGM == BGM::MAOU1 || _BefBGM == BGM::MAOU2 || _BefBGM == BGM::MAOU3)
					{
						// �ʏ�̋�ɖ߂��B
						INSTANCE(SceneManager)->GetSky()->SetSunMode(Sky::SunMode::Transition, Sky::SunMode::Move, 20.0f, 2.0f);
						_StartMoveMoonColor(_defaultColor,0.5f);
						_GameLight->SetIsPointLight(false);
						_Ground->SetIsDL(true);
					}
					_splitMaouzyou->TargetLost();
					_splitMaouzyou->SetActive(false);
					_splitWorld->TargetLost();
					_splitWorld->SetActive(true);
					_nowSplitSpace = _splitWorld;

					_isMaouzyou = false;
				}
			}
		}

		// ��̐؂�ւ��B
		{
			if (bgm == BGM::MAOU1) {
				// ������p�̋�ɕύX�B
				INSTANCE(SceneManager)->GetSky()->SetSunMode(Sky::SunMode::Transition, Sky::SunMode::Stop, /*225.0f*/225.0f/*205.0f*/, 10.0f);
				_StartMoveMoonColor(_defaultColor, 0.5f);
				_GameLight->SetIsPointLight(true);
				_Ground->SetIsDL(false);
			}
			else if (bgm == BGM::MAOU2) {
				// ������p�̋�ɕύX�B
				INSTANCE(SceneManager)->GetSky()->SetSunMode(Sky::SunMode::Transition, Sky::SunMode::Stop, /*225.0f*/215.0f/*205.0f*/, 10.0f);
				_StartMoveMoonColor(_defaultColor, 0.5f);
				_GameLight->SetIsPointLight(true);
				_Ground->SetIsDL(false);
			}
			else if (bgm == BGM::MAOU3) {
				// ������p�̋�ɕύX�B
				INSTANCE(SceneManager)->GetSky()->SetSunMode(Sky::SunMode::Transition, Sky::SunMode::Stop, /*225.0f*/195.0f/*205.0f*/, 10.0f);
				//_StartMoveMoonColor(Color(2.4f, 1.0f, 1.0f), 0.5f);

				_StartMoveMoonColor(Color(2.4f, 0.75f, 0.75f), 0.5f);

				_GameLight->SetIsPointLight(true);
				_Ground->SetIsDL(false);
			}
		}
	}

	if (_BGM != bgm)
	{
		if (static_cast<int>(bgm) < ARRAYSIZE(LocationMapCode)) {
			_worldMap->OpenTownName(LocationMapCode[static_cast<int>(bgm)]);	// �}�b�v�̒n�����J���B
		}

		if (_BGM < BGM::BOSS || bgm >= BGM::BOSS) {
			// ������̎��͊O������w��������܂ŕύX���Ȃ��B
			// ���������S����ʂ̃{�XBGM�̏ꍇ�͕ύX����B

			//BGM�ύX
			_BGM = bgm;
			//���݂�BMG�X�g�b�v
			_GameBGM->Stop();
			//�T�E���h��ݒ�B
			_GameBGM = _SoundBGM[static_cast<int>(_BGM)];
			//�Đ�
			_GameBGM->Play(true);
		}
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

void GameScene::_MoveMoonColor() {
	Plate* Moon = INSTANCE(SceneManager)->GetSky()->GetMoon();
	Color c = Moon->GetBlendColor();
	float deltaTime = Time::DeltaTime();


	if (fabsf(c.r - _targetMoonColor.r) <= 0.0001f) {
		c.r = _targetMoonColor.r;
	}
	else {
		c.r += (_colorOffset.r * _colorMoveSpeed * deltaTime);
	}

	if (fabsf(c.g - _targetMoonColor.g) <= 0.0001f) {
		c.g = _targetMoonColor.g;
	}
	else {
		c.g += (_colorOffset.g * _colorMoveSpeed * deltaTime);
	}

	if (fabsf(c.b - _targetMoonColor.b) <= 0.0001f) {
		c.b = _targetMoonColor.b;
	}
	else {
		c.b += (_colorOffset.b * _colorMoveSpeed * deltaTime);
	}

	Moon->SetBlendColor(c);
}

void GameScene::_StartMoveMoonColor(const Color& target, float time) {
	_targetMoonColor = target;	// ���̐F�ڕW�l�B
	_colorOffset = _targetMoonColor - INSTANCE(SceneManager)->GetSky()->GetMoon()->GetBlendColor();	// �ڕW�̐F�܂ł̍����B
	_colorMoveSpeed = time;				// �F���ω����鑬���B
}
