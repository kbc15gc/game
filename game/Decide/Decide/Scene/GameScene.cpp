#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameObject\Camera\GameCamera.h"

#include "GameObject\Ground\Ground.h"
#include "Ocean.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject\Enemy\BossDrarian.h"

#include "GameObject\History\HistoryManager.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"
#include "GameObject\History\HistoryMenu\HistoryMenu.h"
#include "GameObject\History\Chip.h"

#include "GameObject\Village\Shop.h"
#include "GameObject\Village\ItemManager.h"

#include "GameObject\Camera\PlayerCamera.h"
#include "GameObject\Camera\ThirdPersonCamera.h"
#include "GameObject\Camera\FreeCamera.h"
#include "GameObject\Enemy\EnemyManager.h"
#include "GameObject\SplitSpace.h"

ImageObject* g_depth;

void GameScene::Start()
{
	//�Q�[�����C�g����
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	//�v���C���[����
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);


	//�v���C���[�J��������
	GameCamera* playerCamera = INSTANCE(GameObjectManager)->AddNew<PlayerCamera>("PlayerCamera", 8);
	playerCamera->ActiveCamera();

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

	// ��ԕ��������B
	INSTANCE(GameObjectManager)->AddNew<SplitSpace>("SplitSpace", System::MAX_PRIORITY);

	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�C����.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);

	// �G�l�~�[�}�l�[�W���[�������B
	INSTANCE(EnemyManager)->Start();

	// �{�X�����B
	INSTANCE(GameObjectManager)->AddNew<BossDrarian>("Drarian", 1);
	
	FOR(i,ChipID::ChipNum)
	{
		//���j�`�b�v
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 1);
		chip->SetChipID((ChipID)i);
	}

	//���j���[
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//���j��
	INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 2);

	INSTANCE(HistoryManager)->Start();
	//���j�Ő��������I�u�W�F�N�g�����B
	INSTANCE(HistoryManager)->CreateObject();

	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	INSTANCE(ItemManager)->LoadAllData();
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	shop->OpenShop(0);

	_WorldSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldSE->InitStreaming("Asset/Sound/Battle_BGM.wav");
	_WorldSE->Play(true);

	//�V���h�E�}�b�v�L��.
	_isShadowMap = true;
	//���}�b�v�L��.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable(playerCamera->GetComponent<Camera>(), light->GetComponent<Light>());


	/*g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetDepthofField().GetDepthRenderTarget()->texture);
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);*/

}

void GameScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//�^�C�g���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("TitleScene",true);
		return;
	}

	Collision *coll;
	if(coll = INSTANCE(GameObjectManager)->mainCamera->GetClickCollision(1000.0f, (int)fbCollisionAttributeE::CHARACTER))
	{
		int a = 0;
	}
}