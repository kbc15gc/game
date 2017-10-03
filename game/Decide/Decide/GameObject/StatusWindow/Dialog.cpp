/**
* �_�C�A���O�N���X�̎���.
*/
#include"stdafx.h"
#include"Dialog.h"

namespace
{
	wchar_t* commandName[]=
	{
		L"��������",
		L"�̂Ă�",
	};
}

/**
* ������.
*/
void Dialog::Awake()
{
	_BackWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("", 9);
	_BackWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	_BackWindow->transform->SetParent(transform);
	_BackWindow->SetPivot(0.0f, 0.0f);
	_BackWindow->SetSize(Vector2(200.0f, 150.0f));
}

/**
* ������.
*/
void Dialog::Init(Item::ItemCodeE code)
{
	Vector3 pos = Vector3(10.0f, 10.0f, 0.0f);
	if (Item::ItemCodeE::Item == code)
	{
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("", 9);
		text->Initialize(L"�g��", 30.0f);
		text->transform->SetParent(_BackWindow->transform);
		text->transform->SetLocalPosition(pos);
		text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
		pos.y += 35.0f;
		_CommandList.push_back(text);
	}
	for (int i = 0; i < 2; i++)
	{
		TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("", 9);
		text->Initialize(commandName[i], 30.0f);
		text->transform->SetParent(_BackWindow->transform);
		text->transform->SetLocalPosition(pos);
		text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
		pos.y += 35.0f;
		_CommandList.push_back(text);
	}

	SetActive(false, true);
}

/**
* �X�V.
*/
Dialog::DialogCommand Dialog::InputUpdate()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_B))
	{
		SetActive(false, true);
	}

	return DialogCommand::None;
}

/**
* �L���ɂ���.
*/
void Dialog::Enable(Item2D * item)
{
	SetActive(true, true);
	transform->SetLocalPosition(item->transform->GetPosition());
	_Item = item->GetItemData();
}
