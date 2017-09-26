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
	
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BackWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Hotbar.png"));
	backWindow->transform->SetParent(transform);
	backWindow->SetSize(Vector2(420.0f, 50.0f));

	//アイテム名表示の初期化.
	_ItemNameText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ItemName", 9);
	_ItemNameText->Initialize(L"", 30.0f);
	_ItemNameText->SetFormat(fbText::TextFormatE::LEFT);
	_ItemNameText->transform->SetParent(backWindow->transform);
	_ItemNameText->transform->SetLocalPosition(Vector3(-180.0f, -15.0f, 0.0f));

	_ItemCountText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ItemCount", 9);
	_ItemCountText->Initialize(L"", 30.0f);
	_ItemCountText->SetFormat(fbText::TextFormatE::RIGHT);
	_ItemCountText->transform->SetParent(backWindow->transform);
	_ItemCountText->transform->SetLocalPosition(Vector3(180.0f, -15.0f, 0.0f));

}

/**
* アイテムデータの設定.
*/
void Item2D::SetItemData(HoldItemBase * item)
{
	if (item != nullptr)
	{
		_ItemNameText->SetString(item->GetInfo()->Name);
		char count[100] = { "" };
		if (Item::ItemCodeE::Item==item->GetInfo()->TypeID) {
			sprintf(count, "x%d",static_cast<ConsumptionItem*>(item)->GetHoldNum());
			_ItemCountText->SetString(count);
		}
	}
}
