/**
* �v���C���[�p�����[�^UI�N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"

#include"../Player/Player.h"

/**
* �v���C���[�p�����[�^UI�N���X.
*/
class PlayerParameterUI : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	PlayerParameterUI(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~PlayerParameterUI()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void LateUpdate()override;

private:

	Player* _Player = nullptr;
	
	TextObject* _LvText = nullptr;

	ImageObject* _HpBar = nullptr;
	ImageObject* _ExpBar = nullptr;

	float _NextHpRate = 0.0f;
	float _NowHpRate = 0.0f;

	float _NextExpRate = 0.0f;
	float _NowExpRate = 0.0f;
};