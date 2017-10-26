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

		//スキンモデルデータ作成。
		SkinModelData* modeldata = new SkinModelData();
		// モデルデータ読み込み。
		modeldata->CloneModelData(SkinModelManager::LoadModel("LastBoss.X"), Anim);

		//モデルコンポーネントにモデルデータを設定。
		Model->SetModelData(modeldata);

		Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

		// アニメーションコンポーネントにアニメーションの終了時間設定。
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