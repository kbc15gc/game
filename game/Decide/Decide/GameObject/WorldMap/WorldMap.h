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

class SplitSpace;

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
private:
	/** ���C�g. */
	Light* _Light = nullptr;
};

/**
* �X�e�[�^�X��ʃN���X.
*/
class WorldMap : public GameObject
{

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

	///**
	//* �L����.
	//*/
	//void OnEnable()override;

	///**
	//* ������.
	//*/
	//void OnDisable()override;

private:

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;
	ThirdPersonCamera* _camera = nullptr;	// �}�b�v�p�̘��ՃJ�����B
	MapLight* _mapLight = nullptr;
	bool _isChangeFrame = false;	// ���[���h�}�b�v�̊J��Ԃ��؂�ւ�����t���[�����B
	Light* _saveLight = nullptr;
	SplitSpace* _split = nullptr;
	GameObject* _maouSiro = nullptr;
};