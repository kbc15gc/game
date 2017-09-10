/**
* �X�e�[�^�X��ʃN���X�̒�`.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"Parameter\ParameterRender.h"

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

	/** �p�����[�^���X�g. */
	vector<ParameterRender*> _ParameterRenderList;

	/** �A�C�e���\�����. */
	vector<GameObject*> _ItemWindowList;

};