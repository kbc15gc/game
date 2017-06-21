#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameCamera.h"

#include "Ground.h"
#include "Ocean.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"

#include "GameObject\Village\HistoryMenu.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "GameObject\Village\HistoryMenuSelect.h"

#include "GameObject\Village\Shop.h"
#include "GameObject\Village\ItemManager.h"
#include "GameObject\HistoryChip\Chips.h"

#include "ThirdPersonCamera.h"
#include "FreeCamera.h"

ImageObject* g_depth;

void GameScene::Start()
{
	//�Q�[�����C�g����
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);
	//�Q�[���J��������
	GameCamera* camera = INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 8);
	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�C����.
	INSTANCE(GameObjectManager)->AddNew<Ocean>("Ocean", 7);
	//�v���C���[����
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// �G���G�l�~�[�v���g�����B
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 9);

	FOR(i,ChipID::NUM)
	{
		//���j�`�b�v
		Chips* chip = INSTANCE(GameObjectManager)->AddNew<Chips>("Chip", 1);
		chip->SetChipID((ChipID)i);
	}

	//���j���[
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//���j��
	HistoryBook* book = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 1);
	//���j���̐e�Ƀv���C���[��ݒ�B
	book->transform->SetParent(player->transform);

	//���j���[�Z���N�g
	INSTANCE(GameObjectManager)->AddNew<HistoryMenuSelect>("HistoryMenuSelect", 9);

	//���j�Ő��������I�u�W�F�N�g�����B
	INSTANCE(HistoryManager)->CreateObject();

	//�ӂ���J�����̐����B
	INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);

	//�t���[�J�����̐����B
	INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);



	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	INSTANCE(ItemManager)->LoadItemData();
	Shop* shop = INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);
	shop->OpenShop(0);

	_WorldSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldSE", 9);
	_WorldSE->InitStreaming("Asset/Sound/Battle_BGM.wav");
	_WorldSE->Play(true);

	//�V���h�E�}�b�v�L��.
	_isShadowMap = true;
	//���}�b�v�L��.
	_isEnvironmentMap = true;

	INSTANCE(SceneManager)->GetSky()->SetEnable(camera->GetComponent<Camera>(), light->GetComponent<Light>());

	g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetDepthofField().GetDepthRenderTarget()->texture);
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);

}

void GameScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_BACK);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)))
	{
		//�t�F�[�h�C���J�n
		StartFade(true);
	}
	//�t�F�[�h�C������
	if (_FadeState == fbScene::FadeStateE::INEND)
	{
		//�^�C�g���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("TitleScene");
		return;
	}
}