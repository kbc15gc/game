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

void GameScene::Start()
{
	//�v���C���[����
	INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	//�Q�[���J��������
	INSTANCE(GameObjectManager)->AddNew<GameCamera>("GameCamera", 2);
	//�Q�[�����C�g����
	INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 0);
	//�e�J��������
	INSTANCE(GameObjectManager)->AddNew<GameShadowCamera>("GameShadowCamera", 2);
	//�n�ʐ���
	INSTANCE(GameObjectManager)->AddNew<Ground>("Ground", 1);
	//�󐶐�
	INSTANCE(GameObjectManager)->AddNew<Sky>("Sky", 1);
	// �G���G�l�~�[�v���g�����B
	INSTANCE(GameObjectManager)->AddNew<Enemy>("EnemyProt",1);
	//�΂̗��j�`�b�v
	INSTANCE(GameObjectManager)->AddNew<FireChip>("FireChip", 1);


	//ImageObject* showDepth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShowDepth", 4);
	//showDepth->SetTexture(INSTANCE(RenderTargetManager)->GetRTTextureFromList(RTIdxE::SHADOWDEPTH));
	//showDepth->SetPivot(Vector2(0.0f, 0.0f));

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