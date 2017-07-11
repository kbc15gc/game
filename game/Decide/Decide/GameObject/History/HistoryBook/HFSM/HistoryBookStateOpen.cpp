/**
* ���j�����J����ԃN���X�̎���.
*/
#include "stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateOpen.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateOpen::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Open, 0.2f, 1);

}

/**
* �X�V.
*/
void HistoryBookStateOpen::Update()
{
	//�{���J����������{���J���Ă����ԂŌŒ�B
	if (!_HistoryBook->GetIsPlay())
	{
		//�J������A�j���[�V�����̍Đ����I�������J������ԂɕύX�B
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Idol);
	}
}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateOpen::Exit()
{
	_HistoryBook->OpenPage();
}