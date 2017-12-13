/**
* ��̌��N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\Plate.h"
#include"GameObject\Camera\PlayerCamera.h"
#include"../HistoryMenu/HistoryMenu.h"

/**
* ��̌��N���X.
*/
class MysteryLight : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	MysteryLight(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~MysteryLight()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

	void SetActive(const bool act, const bool child = false)override;

private:

	float _LightTime = 2.5f;
	float _LocalTime = 0.0f;

	/** ���C�g�p�摜. */
	ImageObject* _LightPlate = nullptr;

	PlayerCamera* _PlayerCamera = nullptr;

	HistoryMenu* _HistoryMenu = nullptr;

	float _Alpha = 1.0f;

};