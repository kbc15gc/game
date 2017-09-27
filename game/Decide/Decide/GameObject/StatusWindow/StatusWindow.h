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

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;

	/** �A�C�e���\�����. */
	vector<ItemWindow*> _ItemWindowList;

	int _WindowCount = 3;
	int _NowSelectWindow = 0;

};