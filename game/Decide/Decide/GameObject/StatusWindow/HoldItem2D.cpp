/**
* �����A�C�e����2D�\���N���X�̎���.
*/
#include"stdafx.h"
#include"HoldItem2D.h"

/**
* ������.
*/
void HoldItem2D::Awake()
{
	//�w�i�摜.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel1.jpg"));
	backWindow->transform->SetParent(transform);
	backWindow->SetSize(Vector2(50.0f, 50.0f));
	_ItemIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	_ItemIconImage->transform->SetParent(backWindow->transform);

	_ItemName = INSTANCE(GameObjectManager)->AddNew<TextObject>("", 9);
	_ItemName->Initialize(L"�ݒ�Ȃ�", 30.0f);
	_ItemName->transform->SetParent(backWindow->transform);
	_ItemName->transform->SetLocalPosition(30.0f, 0.0f, 0.0f);
}
