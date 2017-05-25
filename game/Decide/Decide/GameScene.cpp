#include"stdafx.h"
#include "GameScene.h"

#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"

#include "GameLight.h"
#include "GameCamera.h"
#include "GameShadowCamera.h"

#include "Ground.h"
#include "Sky.h"

#include "GameObject/Player/Player.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject/HistoryChip/FireChip.h"
#include "GameObject\HistoryChip\TetuChip.h"
#include "GameObject\HistoryChip\AburaChip.h"

#include "GameObject\Village\HistoryMenu.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "GameObject\Village\HistoryMenuSelect.h"

#include "GameObject\Village\Shop.h"
ImageObject* g_depth;

void GameScene::Start()
{
	//�Q�[�����C�g����
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);
	//�Q�[���J��������
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 8);
	//�e�J��������
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 8);
	//�󐶐�
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 0);
	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�v���C���[����
	Player* player = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// �G���G�l�~�[�v���g�����B
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
	//�΂̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);
	//�S�̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<TetuChip>("TetuChip", 1);
	//���̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<AburaChip>("AburaChip", 1);
	//���j���[
	INSTANCE(GameObjectManager)->AddNew<HistoryMenu>("HistoryMenu", 9);
	//���j��
	HistoryBook* book = INSTANCE(GameObjectManager)->AddNew<HistoryBook>("HistoryBook", 1);
	//���j���[�Z���N�g
	INSTANCE(GameObjectManager)->AddNew<HistoryMenuSelect>("HistoryMenuSelect", 9);

	g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(0));
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(false);

	//���j�Ő��������I�u�W�F�N�g�����B
	INSTANCE(HistoryManager)->CreateObject();

	//���j���̐e�Ƀv���C���[��ݒ�B
	book->transform->SetParent(player->transform);

	INSTANCE(GameObjectManager)->AddNew<Shop>("", 0);

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