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

ImageObject* depth;

void GameScene::Start()
{
	//�Q�[�����C�g����
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 0);
	//�Q�[���J��������
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 10);
	//�e�J��������
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 10);
	//�󐶐�
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 0);
	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�v���C���[����
	INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	// �G���G�l�~�[�v���g�����B
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt",1);
	//�΂̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);

	depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(1));
	depth->SetPivot(Vector2(0, 0));
	depth->SetActive(false);
}

void GameScene::Update()
{
	//�X�^�[�g�{�^���̉����m�F
	bool flag = INSTANCE(InputManager)->IsPushButtonAll(XINPUT_GAMEPAD_START);
	//�G���^�[�L�[
	if ((flag || KeyBoardInput->isPush(DIK_RETURN)) && !_ChangeScene)
	{
		//�`�F���W�V�[���t���O��true
		_ChangeScene = true;
		//�t�F�[�h�J�n
		SetFade(true);
	}
	if (_ChangeScene &&	//�G���^�[�L�[�������ꂽ
		!_IsFade)		//�t�F�[�h�I��
	{
		//�^�C�g���V�[���ֈڍs
		INSTANCE(SceneManager)->ChangeScene("TitleScene");
		//�V�[���`�F���W����
		_ChangeScene = false;
		return;
	}
}