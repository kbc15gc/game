/**
* �A�C�e���\����ʃN���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\ItemManager.h"
#include"Item2D.h"

/**
* �A�C�e���\����ʃN���X.
*/
class ItemWindow : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	ItemWindow(char* name): 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ItemWindow()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �����ŌĂԏ�����.
	*/
	void Init(Item::ItemCodeE code,char* name)
	{
		_ItemCode = code;
		_WindowName->SetText(name);
	}

	/**
	* �X�V.
	*/
	void Update()override;

private:

	/**
	* ����.
	*/
	void Input();

private:

	/** �Z���T�C�Y. */
	static const int ItemCellSize = 10;

	/** �E�B���h�E���\��. */
	TextObject* _WindowName = nullptr;

	/** �A�C�e���`�惊�X�g. */
	vector<Item2D*> _Item2DList;

	/** �A�C�e���R�[�h. */
	Item::ItemCodeE _ItemCode;

	/** ���ݑI�𒆂̃A�C�e��. */
	int _NowSelectItem = 0;
	/** �Z���N�g�J�[�\��. */
	ImageObject* _SelectCursor = nullptr;

};