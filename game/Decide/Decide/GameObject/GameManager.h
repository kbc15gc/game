/**
* �Q�[���Ǘ��N���X�̒�`.
*/
#pragma once

#include"StatusWindow\StatusWindow.h"

/**
* �Q�[���Ǘ��N���X.
* �F�X�Ǘ������悤���Ȃ�����.
*/
class GameManager : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	GameManager(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~GameManager()
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

	/** �X�e�[�^�X��ʃN���X�̃|�C���^. */
	StatusWindow* _StatusWindow = nullptr;

};