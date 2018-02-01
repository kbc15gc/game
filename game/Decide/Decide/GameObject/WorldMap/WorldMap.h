#pragma once
/**
* �X�e�[�^�X��ʃN���X�̒�`.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include "GameObject\Village\EventManager.h"
#include "GameObject\TextImage\AttentionTextOnly.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Camera\ThirdPersonCamera.h"

#include "GameObject\History\HistoryInfo.h"

class SplitSpace;

// ���[���h�}�b�v�̊J���󋵂��Z�[�u���邽�߂̍\���́B
struct WorldMapSaveData {
	WorldMapSaveData() {
	}
	WorldMapSaveData(int num) {
		openLocation = num;
	}
	int openLocation = -1;
};

//WorldMapSaveData{�\���̂̍\���t�H�[�}�b�g(�����o�ϐ�)�B
// ��1�̃��R�[�h�͈ȉ��̗v�f�ō\�������B
// �v�f�F	�ϐ����B
//			�f�[�^�^�B
//			�f�[�^�̐擪���猩���e�v�f�̈ʒu(�o�C�g)�B
//			�v�f����̃T�C�Y(�o�C�g)�B
const Support::DATARECORD WorldMapSaveDataDecl[] =
{
	{ "openLocation",Support::DataTypeE::INT, offsetof(struct WorldMapSaveData,openLocation), sizeof(WorldMapSaveData::openLocation) }
};

class MapLight :public GameObject{
public:
	MapLight(const char* name) :GameObject(name) {};
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	void Update()override;

	Light* GetLight() {
		return _Light;
	}

	void DefaultAmbient() {
		_Light->SetAmbientLight(_defaultAmbient);
	};
private:
	/** ���C�g. */
	Light* _Light = nullptr;
	Vector3 _defaultAmbient;
};

/**
* �X�e�[�^�X��ʃN���X.
*/
class WorldMap : public GameObject
{
private:

	struct Location {
		ImageObject* icon = nullptr;
		TextObject* name = nullptr;
	};
public:

	/**
	* �R���X�g���N�^.
	*/
	WorldMap(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~WorldMap()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	void Start()override;

	void PreUpdate()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �}�b�v���J���B
	*/
	void Open();

	// �}�b�v�����B
	void Close();

	// ���̖��O���������B
	void OpenTownName(LocationCodeAll code) {
		if (code < LocationCodeAll::DevilKingdom) {
			for (auto& data : _saveData) {
				if (data->openLocation == static_cast<int>(code)) {
					// ���łɊJ���ς݁B
					return;
				}
			}

			char text[256];
			sprintf(text, "[%s]", AllLocationNameList[static_cast<int>(code)].c_str());
			_townPoint[static_cast<int>(code)].name->SetText(text);
			//�V����������ꂽ�X�Ȃ̂Ńf�[�^��ǉ����ĕۑ��B
			_saveData.push_back(unique_ptr<WorldMapSaveData>(new WorldMapSaveData(static_cast<int>(code))));
			Support::OutputCSV<WorldMapSaveData>(filePath, WorldMapSaveDataDecl, ARRAY_SIZE(WorldMapSaveDataDecl), _saveData);
		}
	}
private:

	/** �v���C���[�N���X�̃|�C���^. */
	GameObject* _Player = nullptr;
	ThirdPersonCamera* _camera = nullptr;	// �}�b�v�p�̘��ՃJ�����B
	MapLight* _mapLight = nullptr;
	bool _isChangeFrame = false;	// ���[���h�}�b�v�̊J��Ԃ��؂�ւ�����t���[�����B
	Light* _saveLight = nullptr;
	SplitSpace* _split = nullptr;
	GameObject* _maouSiro = nullptr;

	FogInfo _saveFogInfo;

	ImageObject* _playerPoint = nullptr;
	vector<Location> _townPoint = vector<Location>(static_cast<int>(LocationCodeAll::DevilKingdom));

	vector<unique_ptr<WorldMapSaveData>> _saveData;
	char filePath[256] = "C:\\KBCGame\\ReHistory\\SaveData\\WorldMapOpenLocation.csv";	// �Z�[�u��B

																						//���ʉ��Đ��B
	SoundSource* _openSe = nullptr;
	SoundSource* _closeSe = nullptr;
};

