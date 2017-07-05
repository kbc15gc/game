/**
* ���j���̑ҋ@��ԃN���X�̒�`.
*/
#pragma once

#include "HistoryBookState.h"

//#include"GameCamera.h"
#include "GameObject\Camera\PlayerCamera.h"

/**
* ���j���̑ҋ@��ԃN���X.
*/
class HistoryBookStateIdol : public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateIdol(HistoryBook* historybook);

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateIdol()
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

	/** �Q�[���J�����̃|�C���^. */
	PlayerCamera* _PlayerCamera = nullptr;
	/** �v���C���[�̃|�C���^. */
	Player* _Player = nullptr;

};
