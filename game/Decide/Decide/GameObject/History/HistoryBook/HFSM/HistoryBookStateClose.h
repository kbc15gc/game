/**
* ���j���̕����ԃN���X�̒�`.
*/
#pragma once

#include "HistoryBookState.h"

class Player;

/**
* ���j���̕����ԃN���X.
*/
class HistoryBookStateClose : public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateClose(HistoryBook* historybook);

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateClose()
	{
	}

	/**
	* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
	*/
	void Entry()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* ���̏�Ԃɕς��Ƃ��Ă΂��.
	*/
	void Exit()override;

private:


	/** �Q�[���J�����N���X�̃|�C���^. */
	PlayerCamera* _PlayerCamera = nullptr;

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;

};
