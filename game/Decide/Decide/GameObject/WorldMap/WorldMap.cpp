#include "stdafx.h"
#include "WorldMap.h"
#include "fbEngine\_Nature\Sky.h"
#include "GameObject\Nature\Ocean\Ocean.h"
#include "GameLight.h"
#include "GameObject\Ground\Ground.h"
#include "Scene\GameScene.h"
#include "GameObject\SplitSpace.h"


void MapLight::Awake()
{
	_Light = AddComponent<Light>();

	int num = 4;
	DirectionalLight* Dl[4];
	FOR(i, num)
	{
		Dl[i] = new DirectionalLight();
	}

	Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//��
	Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//��
	Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//��
	Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//��

	Dl[0]->SetColor(Color(0.5f, 0.5f, 0.5f, 1.0f));	//��
	Dl[1]->SetColor(Color(0.3f, 0.3f, 0.3f, 1.0f));	//�E
	Dl[2]->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));	//��
	Dl[3]->SetColor(Color(0.1f, 0.1f, 0.1f, 1.0f));	//��

	FOR(i, num)
	{
		_Light->AddLight(Dl[i]);
	}


	//ShadowMap* shadow = INSTANCE(SceneManager)->GetShadowMap();
	//shadow->SetNear(1.0f);
	//shadow->SetFar(100.0f);
}

/**
* ������.
*/
void MapLight::Start()
{
	//INSTANCE(SceneManager)->GetSky()->SetDayAmbientLight(Vector3(0.5f, 0.5f, 0.5f));
	//INSTANCE(SceneManager)->GetSky()->SetNightAmbientLight(Vector3(0.3f, 0.3f, 0.3f));
}

void MapLight::Update()
{
}

/**
* ������.
*/
void WorldMap::Awake() {
	//�ӂ���J�����̐����B
	_camera = INSTANCE(GameObjectManager)->AddNew<ThirdPersonCamera>("ThirdPersonCamera", 8);
	_mapLight = INSTANCE(GameObjectManager)->AddNew<MapLight>("WorldMapLight",7);

	// �Q�[���J�n���͔�A�N�e�B�u�B
	this->SetActive(false);
	_isChangeFrame = false;

}

/**
* �X�V.
*/
void WorldMap::PreUpdate() {
	// �t���O�������B
	_isChangeFrame = false;
}

/**
* �X�V.
*/
void WorldMap::Update() {
	if (VPadInput->IsPush(fbEngine::VPad::ButtonSelect) || VPadInput->IsPush(fbEngine::VPad::ButtonB)) {
		Close();
	}
}

/**
* �}�b�v���J���B
*/
void WorldMap::Open() {
	//if (!_isChangeFrame) {
		// �����t���[���Ő��Close�֐����Ă΂�Ă���΂��̏����͖����B

		// �}�b�v���J���������Ă΂ꂽ�̂ŃA�N�e�B�u���B
		this->SetActive(true);
		// ���݂̃J������ۑ��B
		_camera->SetNextCamera(static_cast<GameCamera*>(INSTANCE(GameObjectManager)->mainCamera->gameObject));
		// ���ՃJ�����ɐ؂�ւ��B
		_camera->ActiveCamera();
		INSTANCE(SceneManager)->GetSky()->SetDisable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(false);
		//INSTANCE(SceneManager)->GetBloom().SetEnable(false);
		//INSTANCE(SceneManager)->GetDepthofField().SetEnable(false);
		_saveLight = INSTANCE(GameObjectManager)->mainLight;
		//INSTANCE(GameObjectManager)->mainLight = _mapLight->GetLight();
		INSTANCE(GameObjectManager)->FindObject("Ground")->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);
		_split = static_cast<GameScene*>(INSTANCE(SceneManager)->GetNowScene())->GetNowSplitSpace();
		if (_split) {
			_split->TargetLost();
			_split->SetActive(false);
		}

		_maouSiro = INSTANCE(GameObjectManager)->FindObject("MaouSiro.X");
		if (_maouSiro)
		{
			_maouSiro->SetActive(false);
		}

		_isChangeFrame = true;
	//}
}

// �}�b�v�����B
void WorldMap::Close() {
	if (!_isChangeFrame) {
		// �����t���[���Ő��Open�֐����Ă΂�Ă���΂��̏����͖����B

		this->SetActive(false);
		if (_camera->GetNextCamera()) {
			// ���̃J�����ɖ߂��B
			_camera->GetNextCamera()->ActiveCamera();
		}

		INSTANCE(SceneManager)->GetSky()->SetEnable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(true);
		INSTANCE(GameObjectManager)->FindObject("Ground")->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

		//INSTANCE(GameObjectManager)->mainLight = _saveLight;

		if (_split) {
			_split->SetActive(true);
		}
		if (_maouSiro)
		{
			_maouSiro->SetActive(true);
		}

		// ���[���h�}�b�v���[�h���I���������Ƃ��C�x���g�}�l�[�W���[�ɒʒm�B
		INSTANCE(EventManager)->NotifyEndEvent();

		_isChangeFrame = true;
	}
}
