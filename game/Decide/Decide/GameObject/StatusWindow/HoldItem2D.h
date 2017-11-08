/**
* �����A�C�e����2D�\���N���X�̒�`.
*/
#pragma once

#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

/**
* �����A�C�e����2D�\���N���X.
*/
class HoldItem2D : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HoldItem2D(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HoldItem2D()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �����ŌĂԏ�����.
	*
	* @param dir	�A�C�e�����\������(0:�E,1:��).
	*/
	void Init(int dir = 0)
	{
		if (dir == 0)
		{
			_ItemName->SetAnchor(fbText::TextAnchorE::UpperLeft);
			_ItemName->transform->SetLocalPosition(30.0f, -15.0f, 0.0f);
		}
		else
		{
			_ItemName->SetAnchor(fbText::TextAnchorE::UpperRight);
			_ItemName->transform->SetLocalPosition(-30.0f, -15.0f, 0.0f);
		}
	}

	/**
	* �����A�C�e����ݒ�.
	*/
	void SetHoldItem(HoldItemBase* item)
	{
		_HoldItemBase = item;
		if (_HoldItemBase)
		{
			_ItemIconImage->SetActive(true, true);
			_ItemIconImage->SetTexture(LOADTEXTURE(_HoldItemBase->GetInfo()->filePath));
			_ItemIconImage->SetSize(Vector2(45.0f, 45.0f));
			_ItemName->SetActive(true, true);
			_ItemName->SetText(_HoldItemBase->GetInfo()->Name);
		}
		else
		{
			_ItemIconImage->SetActive(false, true);
			_ItemIconImage->SetTexture(nullptr);
			_ItemName->SetActive(true, true);
			_ItemName->SetText(L"�ݒ�Ȃ�");
		}
	}

private:

	/** �����A�C�e���̃|�C���^. */
	HoldItemBase* _HoldItemBase = nullptr;

	/** �A�C�R���\��. */
	ImageObject* _ItemIconImage = nullptr;

	/** �A�C�e�����\��. */
	TextObject* _ItemName = nullptr;

};