#include "GameScene.h"
#include "GameCamera.h"
#include "GameLight.h"
#include "GameShadowCamera.h"
#include "GameObject/Player/Player.h"
#include "Ground.h"
#include "Sky.h"
#include "fbEngine/_Object/_GameObject/ImageObject.h"
#include "fbEngine/_Object/_GameObject/TextObject.h"
#include "fbEngine/_Object/_GameObject/SoundSource.h"
#include "GameObject\Enemy\Enemy.h"
#include "GameObject/HistoryChip/FireChip.h"
#include "GameObject\Village\HistoryManager.h"

ImageObject* depth;

void GameScene::Start()
{
	//�Q�[�����C�g����
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 0);
	//�Q�[���J��������
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 0);
	//�e�J��������
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 0);
	//�󐶐�
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 0);
	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�v���C���[����
	INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// �G���G�l�~�[�v���g�����B
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt", 1);
	//�΂̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);
	//���j�Ǘ�
	INSTANCE(GameObjectManager)->AddNew<HistoryManager>("HistoryManager", 0);

	//depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	//depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(1));
	/*ImageObject* depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	depth->SetTexture(INSTANCE(RenderTargetManager)->GetRTTextureFromList(RTIdxE::SHADOWDEPTH));
	depth->SetPivot(Vector2(0, 0));
	depth->SetActive(false);*/
}

void GameScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
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