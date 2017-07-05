/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X�̒�`.
*/
#pragma once

#include"..\HistoryInfo.h"

/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X.
*/
class Chip2D : public GameObject
{
public:

	/**
	* �T�C�Y�R�[�h.
	*/
	enum class SizeCodeE
	{
		Select,		//!< �I��.
		NoSelect,	//!< �I���O.
		SizeCodeNum,	//!< �T�C�Y��.
	};



public:

	/**
	* �R���X�g���N�^.
	*/
	Chip2D(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Chip2D()
	{
	}

	/**
	* ������.
	*/
	void Start(ChipID chipID);

	/**
	* �T�C�Y��ݒ�.
	*/
	void SetSize(SizeCodeE size);

	ChipID GetChipID()
	{
		return _ChipID;
	}

private:

	/** �`�b�v�\���N���X. */
	Sprite* _ChipSprite = nullptr;

	/** �`�b�vID. */
	ChipID _ChipID = ChipID::None;

};