/**
* �A�C�e���\����ʃN���X�̎���.
*/
#include"stdafx.h"
#include"ItemWindow.h"

#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* ������.
*/
void ItemWindow::Awake()
{
	//�o�b�N�̔w�i.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel4.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.06f);
	backWindow->SetPivot(0.0f, 0.5f);
	backWindow->transform->SetParent(transform);
	backWindow->transform->SetLocalPosition(Vector3(35.0f, 45.0f, 0.0f));
}

/**
* �X�V.
*/
void ItemWindow::Update()
{
}
