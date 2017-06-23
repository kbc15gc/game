/**
* ���j�ύX���j���[��ʃN���X�̒�`.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"

#include"..\HistoryInfo.h"

#include"Chip2D.h"

class HistoryBook;
class TextObject;

/**
* ���j�ύX���j���[��ʃN���X.
* ���j�����J�����Ƃ��ɕ\�������.
*/
class HistoryMenu:public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryMenu(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryMenu()
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

	/**
	* �`�b�v��ǉ�.
	*/
	void AddChip(ChipID chipID);

private:

	/**
	* �\�����̍X�V.
	*/
	void EnableUpdate();

private:

	/** �ꏊ���\���I�u�W�F�N�g. */
	TextObject* _LocationNameRender = nullptr;
	/** ���ݑI�𒆂̏ꏊ. */
	int _NowSelectLocation = 0;

	/** �������Ă���ꏊ�̌�. */
	int _ReleaseLocation = 0;

	/** ���j���N���X�̃|�C���^. */
	HistoryBook* _HistoryBook = nullptr;

	/** �����`�b�v��\������N���X���X�g. */
	vector<Chip2D*> _Chip2DList;

	/** ���ݑI�����Ă��鏊���`�b�v. */
	int _NowSelectChip = 0;

};