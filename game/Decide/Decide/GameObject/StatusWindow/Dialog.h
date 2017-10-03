/**
* �_�C�A���O�N���X�̒�`.
*/
#pragma once

#include"Item2D.h"

/**
* �_�C�A���O�N���X.
*/
class Dialog : public GameObject
{
public:

	/**
	* �R�}���h.
	*/
	enum DialogCommand
	{
		None = -1,		//!< �Ȃ�.
		Equip,			//!< ����.
		Dump,			//!< �̂Ă�.
		UseItem,		//!< �A�C�e���g�p.
		EquipItemUp,	//!< �A�C�e��������.
		EquipItemDwon,	//!< �A�C�e��������.
		EquipItemLeft,	//!< �A�C�e��������.
		EquipItemRight,	//!< �A�C�e��������.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	Dialog(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Dialog()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Init(Item::ItemCodeE code);

	/**
	* �X�V.
	*/
	DialogCommand InputUpdate();

	/**
	* �L���ɂ���.
	*/
	void Enable(Item2D* item);

private:

	/** �A�C�e��. */
	HoldItemBase* _Item = nullptr;

	/** �w�i. */
	ImageObject* _BackWindow = nullptr;

	/** �J�[�\���I�u�W�F�N�g. */
	ImageObject* _CursorObject = nullptr;

	/** �R�}���h���X�g. */
	vector<TextObject*> _CommandList;

};