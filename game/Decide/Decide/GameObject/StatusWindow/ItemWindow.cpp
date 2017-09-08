/**
* アイテム表示画面クラスの実装.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* 初期化.
*/
void ItemWindow::Awake()
{
	//バックの背景.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel4.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.06f);
	backWindow->SetPivot(0.0f, 0.5f);
	backWindow->transform->SetParent(transform);
	backWindow->transform->SetLocalPosition(Vector3(35.0f, 45.0f, 0.0f));
}

/**
* 更新.
*/
void ItemWindow::Update()
{
}
