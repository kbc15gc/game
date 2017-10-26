#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "GameObject\Enemy\LastBoss.h"
#include "GameObject\Player\Player.h"

class TestBoss :public GameObject {
public:
	TestBoss(char* name) :GameObject(name) {

	}
	~TestBoss() {

	}

	void Awake()override {
		SkinModel* Model = AddComponent<SkinModel>();
		Animation* Anim = AddComponent<Animation>();

		//�X�L�����f���f�[�^�쐬�B
		SkinModelData* modeldata = new SkinModelData();
		// ���f���f�[�^�ǂݍ��݁B
		modeldata->CloneModelData(SkinModelManager::LoadModel("LastBoss.X"), Anim);

		//���f���R���|�[�l���g�Ƀ��f���f�[�^��ݒ�B
		Model->SetModelData(modeldata);

		Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

		// �A�j���[�V�����R���|�[�l���g�ɃA�j���[�V�����̏I�����Ԑݒ�B
		Anim->SetAnimationEndTime(1, -1.0);

		Anim->PlayAnimation(1, 0.2f,1);
	}

	void Update()override {

	}
private:

};

class LastBossTestScene : public Scene
{
public:
	LastBossTestScene() {};
	void Start()override;
	void Update()override;
private:
	Player* p;
	TestBoss* _boss = nullptr;
};