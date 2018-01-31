/**
* �Q�[���Ǘ��N���X�̒�`.
*/
#pragma once

#include"StatusWindow\StatusWindow.h"
#include"History\HistoryBook\HistoryBook.h"
#include"Player\Player.h"
#include"Camera\PlayerCamera.h"
#include "GameObject\WorldMap\WorldMap.h"

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

	Player* _Player = nullptr;

	/** �X�e�[�^�X��ʃN���X�̃|�C���^. */
	StatusWindow* _StatusWindow = nullptr;

	/** �q�X�g���[�u�b�N. */
	HistoryBook* _HistoryBook = nullptr;

	PlayerCamera* _PlayerCamera = nullptr;

	// �}�b�v�B
	WorldMap* _worldMap = nullptr;
};