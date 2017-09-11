/**
* アイテム表示画面クラスの実装.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"

/**
* 初期化.
*/
void ItemWindow::Awake()
{
	//バックの背景.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	backWindow->SetSize(Vector2(495.0f, 580.0f));
	backWindow->SetPivot(0.0f, 0.5f);
	backWindow->transform->SetParent(transform);
	backWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	TextObject* windowName = INSTANCE(GameObjectManager)->AddNew<TextObject>("WindowName", 9);
	windowName->Initialize(L"消費アイテム", 30.0f);
	windowName->SetFormat(fbText::TextFormatE::CENTER);
	windowName->transform->SetParent(backWindow->transform);
	windowName->transform->SetLocalPosition(Vector3(250.0f, -280.0f, 0.0f));

	for (int i = 0; i < 8; i++)
	{
		TextObject* itemName = INSTANCE(GameObjectManager)->AddNew<TextObject>("ItemName", 9);
		itemName->Initialize(L"", 30.0f);
		itemName->SetFormat(fbText::TextFormatE::LEFT);
		itemName->transform->SetParent(backWindow->transform);
		itemName->transform->SetLocalPosition(Vector3(20.0f, -250.0f + (i * 35.0f), 0.0f));
		_ItemTextList.push_back(itemName);
	}
}

/**
* 更新.
*/
void ItemWindow::Update()
{
	auto& itemList = INSTANCE(Inventory)->GetInventoryList(ItemManager::ItemCodeE::Item);

	for (int i = 0; i < 8; i++)
	{
		if (itemList.size() > i && itemList[i] != nullptr)
		{
			_ItemTextList[i]->SetString(itemList[i]->_Info->Name);
		}
		else
		{
			_ItemTextList[i]->SetString(L"");
		}
	}
}
