#include"stdafx.h"
#include "LastBossTestScene.h"

#include "GameLight.h"

#include "GameObject\Ground\Ground.h"

#include "GameObject\Camera\FreeCamera.h"

#include"GameObject\GameManager.h"
#include "GameObject\Enemy\LastBoss.h"


//#define _NKMT_


void LastBossTestScene::Start()
{
	p = INSTANCE(GameObjectManager)->AddNew<Player>("Player", 1);
	//�Q�[�����C�g����
	GameLight* light = INSTANCE(GameObjectManager)->AddNew<GameLight>("GameLight", 8);

	INSTANCE(ItemManager)->LoadAllItemData();

	INSTANCE(Inventory)->Initialize();

#ifdef _DEBUG

	//�v���C���[�J��������
	GameCamera* playerCamera = INSTANCE(GameObjectManager)->AddNew<PlayerCamera>("PlayerCamera", 8);
	playerCamera->ActiveCamera();

	////�t���[�J�����̐����B
	//GameCamera* freeCamera = INSTANCE(GameObjectManager)->AddNew<FreeCamera>("FreeCamera", 8);
	//freeCamera->ActiveCamera();

#endif
	// �e�X�g�B
	// ���X�{�X�쐬�B
	_boss = INSTANCE(GameObjectManager)->AddNew<TestBoss>("LastBoss", 1);
	//// �p�����[�^�[�ݒ�B
	//vector<BarColor> Color;
	//Color.push_back(BarColor::Blue);
	//Color.push_back(BarColor::Green);
	//Color.push_back(BarColor::Yellow);
	//Color.push_back(BarColor::Red);
	//vector<int> param = vector<int>(static_cast<int>(CharacterParameter::Param::MAX), 10);
	//enemy->SetParamAll(Color, param);

	INSTANCE(GameObjectManager)->AddNew<GameManager>("GameManager", 0);

	//�V���h�E�}�b�v�L��.
	_isShadowMap = true;
	//���}�b�v�L��.
	_isEnvironmentMap = true;


	/*g_depth = INSTANCE(GameObjectManager)->AddNew<ImageObject>("debug", 4);
	g_depth->SetTexture(INSTANCE(SceneManager)->GetShadowMap()->GetTexture(0));
	g_depth->SetPivot(Vector2(0, 0));
	g_depth->SetSize(g_depth->GetTexture()->Size * 0.5);
	g_depth->SetActive(true);*/
}

void LastBossTestScene::Update()
{
	static int anim = 1;

	p->transform->SetPosition(Vector3::zero);
	_boss->transform->SetPosition(Vector3::zero);
	if (!_boss->GetComponent<Animation>()->GetPlaying()) {
		if (anim == 1) {
			anim = 4;
		}
		else {
			anim = 1;
		}
		_boss->GetComponent<Animation>()->PlayAnimation(anim, 0.2f, 1);
	}
}

