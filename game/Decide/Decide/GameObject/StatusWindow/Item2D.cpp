/**
* アイテム2D表示クラスの実装.
*/
#include"stdafx.h"
#include"Item2D.h"

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\ItemManager\HoldItem\ConsumptionItem.h"

/**
* 初期化.
*/
void Item2D::Awake()
{
	
	_BackWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BackWindow", 9);
	_BackWindow->SetTexture(LOADTEXTURE("UI/Hotbar.png"));
	_BackWindow->transform->SetParent(transform);
	_BackWindow->SetSize(Vector2(420.0f, 50.0f));

	_Icon = INSTANCE(GameObjectManager)->AddNew<ImageObject>("Icon", 9);
	_Icon->SetTexture(nullptr);
	_Icon->transform->SetParent(_BackWindow->transform);
	_Icon->SetSize(Vector2(40.0f, 40.0f));
	_Icon->transform->SetLocalPosition(Vector3(-180.0f, 0.0f, 0.0f));

	//アイテム名表示の初期化.
	_ItemNameText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ItemName", 9);
	_ItemNameText->Initialize(L"", 30.0f);
	_ItemNameText->SetAnchor(fbText::TextAnchorE::MiddleLeft);
	_ItemNameText->transform->SetParent(_BackWindow->transform);
	_ItemNameText->transform->SetLocalPosition(Vector3(-160.0f, -15.0f, 0.0f));

	_ItemCountText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ItemCount", 9);
	_ItemCountText->Initialize(L"", 30.0f);
	_ItemCountText->SetAnchor(fbText::TextAnchorE::MiddleRight);
	_ItemCountText->transform->SetParent(_BackWindow->transform);
	_ItemCountText->transform->SetLocalPosition(Vector3(180.0f, -15.0f, 0.0f));

}

/**
* アイテムデータの設定.
*/
void Item2D::SetItemData(HoldItemBase * item)
{
	if (item != nullptr)
	{
		_Icon->SetTexture(LOADTEXTURE(item->GetInfo()->filePath));
		_Icon->SetSize(Vector2(40.0f, 40.0f));
		
		_ItemNameText->SetText(item->GetInfo()->Name);
		char count[100] = { "" };
		if (Item::ItemCodeE::Item == item->GetInfo()->TypeID) {
			sprintf(count, "x%d", static_cast<ConsumptionItem*>(item)->GetHoldNum());
			_ItemCountText->SetText(count);
		}

		_ItemData = item;
		SetActive(true, true);
	}
	else
	{
		_ItemData = nullptr;
		SetActive(false, true);
	}
}
