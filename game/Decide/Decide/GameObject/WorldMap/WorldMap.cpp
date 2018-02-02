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

	//int num = 4;
	//DirectionalLight* Dl[4];
	//FOR(i, num)
	//{
	//	Dl[i] = new DirectionalLight();
	//}

	//Dl[0]->SetEulerAngles(Vector3(45, 45, 180));	//��
	//Dl[1]->SetEulerAngles(Vector3(0, 0, 180));	//��
	//Dl[2]->SetEulerAngles(Vector3(0, -90, 0));	//��
	//Dl[3]->SetEulerAngles(Vector3(90, 0, 0));	//��

	//Dl[0]->SetColor(Color(0.7f, 0.7f, 0.2f, 0.3f));	//��
	//Dl[1]->SetColor(Color(0.7f, 0.7f, 0.2f, 0.3f));	//�E
	//Dl[2]->SetColor(Color(0.7f, 0.7f, 0.2f, 0.3f));	//��
	//Dl[3]->SetColor(Color(0.7f, 0.7f, 0.2f, 0.3f));	//��

	//FOR(i, num)
	//{
	//	_Light->AddLight(Dl[i]);
	//}
	//_defaultAmbient = Vector3(0.9f, 0.9f, 0.65f);
	_defaultAmbient = Vector3(1.0f, 1.0f, 1.0f);

	_Light->SetAmbientLight(_defaultAmbient);

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

	_playerPoint = INSTANCE(GameObjectManager)->AddNew<ImageObject>("playerMapIcon", 2);
	//_playerPoint->SetTexture(LOADTEXTURE("Blue_Arrow.png"));
	_playerPoint->SetTexture(LOADTEXTURE("PlayerIcon.png"));
	_playerPoint->SetSize(Vector2(40.0f, 40.0f));

	_playerPoint->SetSize(Vector2(40.0f, 40.0f));
	//_playerPoint->SetClipColor(Color(1.0f, 1.0f, 0.0f));
	_playerPoint->SetBlendColor(Color(1.5f,3.0f,0.5f));
	_playerPoint->SetActive(false);

	for (int idx = 0; idx < static_cast<int>(LocationCodeAll::DevilKingdom); idx++) {
		_townPoint[idx].icon = INSTANCE(GameObjectManager)->AddNew<ImageObject>("townMapIcon", 1);
		_townPoint[idx].icon->SetTexture(LOADTEXTURE("t1.png"));
		_townPoint[idx].icon->SetSize(Vector2(30.0f, 30.0f));
		_townPoint[idx].icon->SetActive(false);
		//_townPoint[idx].icon->SetClipColor(Color(0.0f, 0.0f, 0.0f));
		//_townPoint[idx].icon->SetBlendColor(Color(2.0f,1.0f,8.0f));
		_townPoint[idx].icon->SetBlendColor(Color(0.6f, 0.7f, 2.3f));

		_townPoint[idx].name = INSTANCE(GameObjectManager)->AddNew<TextObject>("townMapName", 1);
		_townPoint[idx].name->Initialize(L"[?????]", 25.0f);
		_townPoint[idx].name->transform->SetParent(_townPoint[idx].icon->transform);
		_townPoint[idx].name->SetActive(false);
	}

	_mapFilter = INSTANCE(GameObjectManager)->AddNew<ImageObject>("MapFilter", 0);
	_mapFilter->SetTexture(LOADTEXTURE("WorldMapFilter.png"));
	_mapFilter->SetSize(Vector2(WindowW, WindowH));
	_mapFilter->SetActive(false);
	_mapFilter->transform->SetPosition(Vector3(WindowW * 0.5f, WindowH * 0.5f, 0.0f));

	if (IS_CONTINUE == false) {
		InitSaveData();
	}
	//CSV����I�u�W�F�N�g�̏��ǂݍ��݁B
	Support::LoadCSVData<WorldMapSaveData>(filePath, WorldMapSaveDataDecl, ARRAY_SIZE(WorldMapSaveDataDecl), _saveData);

	for (auto& data : _saveData) {
		char text[256];
		sprintf(text, "[%s]", AllLocationNameList[data->openLocation].c_str());
		_townPoint[data->openLocation].name->SetText(text);
	}

	_openSe = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldMapSE", 0);
	_closeSe = INSTANCE(GameObjectManager)->AddNew<SoundSource>("WorldMapSE", 0);

	//�J�����Ƃ��ƕ������ňقȂ鉹�B
	_openSe->Init("Asset/Sound/UI/Menu.wav");
	_closeSe->Init("Asset/Sound/UI/Menu.wav");

	_BloodEffect = INSTANCE(GameObjectManager)->FindObject("BloodEffect");
}

void WorldMap::Start() {
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
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
	//_mapLight->DefaultAmbient();
	//INSTANCE(GameObjectManager)->mainLight = _mapLight->GetLight();

	if (VPadInput->IsPush(fbEngine::VPad::ButtonSelect) || VPadInput->IsPush(fbEngine::VPad::ButtonB)) {
		Close();
	}
	else {
		Vector3 pos = _Player->transform->GetPosition();
		Vector2 screenPos = _camera->GetCamera()->WorldToScreen(pos);
		_playerPoint->transform->SetPosition(screenPos);
		Quaternion rot = Quaternion::Identity;
		Vector3 dir = _Player->transform->GetForward();
		rot.SetRotation(Vector3::axisZ, atan2f(dir.x, dir.z));
		//Quaternion rot2 = Quaternion::Identity;

		//rot1.SetRotation(Vector3::axisX, D3DXToRadian(90.0f));
		//rot2.CreateVector3ToVector3(_Player->transform->GetForward(), Vector3::front);
		//rot1.Multiply(rot2);
		//char test[256];
		//sprintf(test, "rot = %f\n", rad);
		//OutputDebugString(test);

		_playerPoint->transform->SetRotation(rot);
		//_playerPoint->Set

		for (int idx = 0; idx < static_cast<int>(LocationCodeAll::DevilKingdom); idx++) {
			Vector3 pos = AllLocationPosition[idx];
			Vector2 screenPos = _camera->GetCamera()->WorldToScreen(pos);
			//if (screenPos.x >= -99999.0f) {
			//	OutputDebugString("��ʂɉf���ĂȂ��炵�����I\n");
			//}

			_townPoint[idx].icon->transform->SetPosition(screenPos);
			_townPoint[idx].name->transform->SetLocalPosition(Vector3(0.0f, -48.0f, 0.0f));
		}
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

		// ����Ȃ����̂����낢���A�N�e�B�u�ɂ��Ă����B
		_BloodEffect->SetActive(false);
		INSTANCE(SceneManager)->GetSky()->SetDisable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(false);
		//INSTANCE(SceneManager)->GetBloom().SetEnable(false);
		//INSTANCE(SceneManager)->GetDepthofField().SetEnable(false);
		_saveLight = INSTANCE(GameObjectManager)->mainLight;
		_mapLight->DefaultAmbient();
		INSTANCE(GameObjectManager)->mainLight = _mapLight->GetLight();
		GameObject* ground = INSTANCE(GameObjectManager)->FindObject("Ground");

		ground->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncNone);
		_saveFogInfo = ground->GetComponent<SkinModel>()->GetFogParam();
		ground->GetComponent<SkinModel>()->SetFogParam(FogFunc::FogFuncNone, 0.0f, 0.0f, Vector4(0.0f, 0.0f, 0.0f, 0.0f));


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

		// �}�b�v�A�C�R���Ƃ����A�N�e�B�u���B
		_playerPoint->SetActive(true);
		_mapFilter->SetActive(true);
		for (int idx = 0; idx < static_cast<int>(LocationCodeAll::DevilKingdom); idx++) {
			_townPoint[idx].icon->SetActive(true);
			_townPoint[idx].name->SetActive(true);
		}

		_openSe->Play(false);

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

		// ��A�N�e�B�u�ɂ������̂����ʂ�ɁB
		_BloodEffect->SetActive(true);
		INSTANCE(SceneManager)->GetSky()->SetEnable();
		//INSTANCE(GameObjectManager)->FindObject("Ocean")->SetActive(true);
		GameObject* ground = INSTANCE(GameObjectManager)->FindObject("Ground");
		ground->GetComponent<SkinModel>()->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);
		ground->GetComponent<SkinModel>()->SetFogParam(_saveFogInfo);

		INSTANCE(GameObjectManager)->mainLight = _saveLight;

		if (_split) {
			_split->SetActive(true);
		}
		if (_maouSiro)
		{
			_maouSiro->SetActive(true);
		}


		// �}�b�v�A�C�R���Ƃ����A�N�e�B�u�ɁB
		_playerPoint->SetActive(false);
		_mapFilter->SetActive(false);

		for (int idx = 0; idx < static_cast<int>(LocationCodeAll::DevilKingdom); idx++) {
			_townPoint[idx].icon->SetActive(false);
			_townPoint[idx].name->SetActive(false);
		}


		// ���[���h�}�b�v���[�h���I���������Ƃ��C�x���g�}�l�[�W���[�ɒʒm�B
		INSTANCE(EventManager)->NotifyEndEvent();

		_closeSe->Play(false);

		_isChangeFrame = true;
	}
}
