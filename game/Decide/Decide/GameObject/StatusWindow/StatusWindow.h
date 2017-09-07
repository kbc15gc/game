/**
* �X�e�[�^�X��ʃN���X�̒�`.
*/
#pragma once

#include"GameObject\Player\Player.h"
#include"Parameter\ParameterRender.h"
#include"fbEngine\_Object\_Component\_2D\Sprite.h"

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

	/** �E�B���h�E�\���N���X. */
	Sprite* _WindowSprite = nullptr;

	/** �p�����[�^���X�g. */
	vector<ParameterRender*> _ParameterRenderList;

};