/**
* 装備アイテムの2D表示クラスの定義.
*/
#pragma once

#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

/**
* 装備アイテムの2D表示クラス.
*/
class HoldItem2D : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	HoldItem2D(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HoldItem2D()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 自分で呼ぶ初期化.
	*
	* @param dir	アイテム名表示方向(0:右,1:左).
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
	* 所持アイテムを設定.
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
			_ItemName->SetText(L"設定なし");
		}
	}

private:

	/** 所持アイテムのポインタ. */
	HoldItemBase* _HoldItemBase = nullptr;

	/** アイコン表示. */
	ImageObject* _ItemIconImage = nullptr;

	/** アイテム名表示. */
	TextObject* _ItemName = nullptr;

};