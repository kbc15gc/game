/**
* �X�e�[�^�X��ʃN���X�̒�`.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"ItemWindow.h"

/**
* �X�e�[�^�X��ʃN���X.
*/
class StatusWindow : public GameObject
{
public:
	enum ShowStatus { LV = 0, HP, MP, ATK, MAT, DEF, MDE, DEX, MONEY, MAX };

public:

	/**
	* �R���X�g���N�^.
	*/
	StatusWindow(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~StatusWindow()
	{
	}

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void Update()override;

private:
	// �X�e�[�^�X�\���쐬�B
	void _CreateShowStatus();
	// �p�����[�^�\���N���X�̃C���X�^���X�ɒl��ݒ�B
	void _ConfigParamRender();
private:

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;
	// �v���C���[�̃��x���B
	int _playerLevel = 0;
	// �Q�[�W�B
	ParameterBar* _ExpBar = nullptr;
	ParameterBar* _HpBar = nullptr;
	ParameterBar* _MpBar = nullptr;

	/** �p�����[�^���X�g. */
	vector<ParameterRender*> _ParameterRenderList;

	/** �A�C�e���\�����. */
	vector<ItemWindow*> _ItemWindowList;

	int _WindowCount = 3;
	int _NowSelectWindow = 0;

};