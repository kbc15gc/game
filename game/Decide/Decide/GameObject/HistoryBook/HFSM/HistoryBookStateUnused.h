/**
* ���j���̖��g�p��ԃN���X�̒�`.
*/
#pragma once

#include "HistoryBookState.h"
#include "GameCamera.h"

class Player;

/**
* ���j���̖��g�p��ԃN���X.
*/
class HistoryBookStateUnused : public IHistoryBookState
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryBookStateUnused(HistoryBook* historybook);

	/**
	* �f�X�g���N�^.
	*/
	~HistoryBookStateUnused()
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
	GameCamera* _GameCamera = nullptr;

	/** �v���C���[�N���X�̃|�C���^. */
	Player* _Player = nullptr;
	/** �v���C���[�̐��ʕ���. */
	Vector3 _PlayerFoward = Vector3::zero;

};
