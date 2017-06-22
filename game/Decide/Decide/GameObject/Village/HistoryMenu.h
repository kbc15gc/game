/**
* ���j�ύX���j���[��ʃN���X�̒�`.
*/
#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class HistoryBook;
class TextObject;

/**
* ���j�ύX���j���[��ʃN���X.
* ���j�����J�����Ƃ��ɕ\�������.
*/
class HistoryMenu:public GameObject
{
public:

	enum MenuNomber
	{
		One = 0,
		Two,
		Three,
	};

	/**
	* �ꏊ�R�[�h.
	*/
	enum class LocationCodeE
	{
		Begin = 0,		//!< �n�܂�̏W��.
		Hunting,		//!< ��̑�.
		Prosperity,		//!< �ɉh�̒�.
		LocationNum,	//!< �ꏊ�̐�.
	};

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

};