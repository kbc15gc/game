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

private:

	/** �A�C�e�����\��. */
	TextObject* _ItemNameText = nullptr;

	/** �A�C�e����. */
	TextObject* _ItemCountText = nullptr;

};