/**
* ���j���̏�Ԃ̊��N���X�̎���.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateUnused.h"

/**
* ��Ԃɐ؂�ւ����Ƃ��Ă΂��.
*/
void HistoryBookStateUnused::Entry()
{
	_HistoryBook->SetEnable(false);
}

/**
* �X�V.
*/
void HistoryBookStateUnused::Update()
{

}

/**
* ���̏�Ԃɕς��Ƃ��Ă΂��.
*/
void HistoryBookStateUnused::Exit()
{

}