/**
* �A�C�e��2D�\���N���X�̒�`.
*/
#pragma once

#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

/**
* �A�C�e��2D�`��N���X.
*/
class Item2D : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	Item2D(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Item2D()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �A�C�e���f�[�^�̐ݒ�.
	*/
	void SetItemData(HoldItemBase* item);

	HoldItemBase* GetItemData()
	{
		return _ItemData;
	}

private:

	HoldItemBase* _ItemData = nullptr;

	/** �w�i. */
	ImageObject* _BackWindow = nullptr;

	/** �A�C�R��. */
	ImageObject* _Icon = nullptr;

	/** �A�C�e�����\��. */
	TextObject* _ItemNameText = nullptr;

	/** �A�C�e����. */
	TextObject* _ItemCountText = nullptr;

};