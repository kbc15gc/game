/**
* �X�e�[�^�X��ʃN���X�̎���.
*/
#include"stdafx.h"
#include"StatusWindow.h"

#include"GameObject\Inventory\Inventory.h"


const int StatusWindow::WindowBackPriorty = 6;

/**
* ������.
*/
void StatusWindow::Awake()
{
	//�v���C���[�N���X���擾.
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//���W�𒆐S�ɐݒ�.
	transform->SetPosition(Vector3(g_WindowSize.x / 2, g_WindowSize.y / 2, 0.0f));

	//�o�b�N�̔w�i.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", WindowBackPriorty);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.3f);
	backWindow->transform->SetParent(transform);

	//�A�C�e���ꗗ�̔w�i.
	ImageObject* itemWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", WindowBackPriorty + 1);
	itemWindow->SetTexture(LOADTEXTURE("UI/Panel5.png"));
	itemWindow->SetSize(Vector2(495.0f, 580.0f));
	itemWindow->SetPivot(0.0f, 0.5f);
	itemWindow->transform->SetParent(transform);
	itemWindow->transform->SetLocalPosition(Vector3(0.0f, 47.0f, 0.0f));

	for (int i = 0; i < _WindowCount; i++)
	{
		ItemWindow* iw = INSTANCE(GameObjectManager)->AddNew<ItemWindow>("ItemWindow", WindowBackPriorty + 3);
		iw->transform->SetParent(transform);
		iw->SetActive(false, true);
		_ItemWindowList.push_back(iw);
	}
	_ItemWindowList[0]->Init(Item::ItemCodeE::Item);
	_ItemWindowList[1]->Init(Item::ItemCodeE::Weapon);
	_ItemWindowList[2]->Init(Item::ItemCodeE::Armor);

	for (int i = 0; i < _WindowCount; i++)
	{
		_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
	}

	// �����̕\���쐬�B
	_MoneyFrame = INSTANCE(GameObjectManager)->AddNew<ImageObject>("MoneyFrame", WindowBackPriorty + 2);
	_MoneyFrame->transform->SetParent(transform);
	_MoneyFrame->transform->SetLocalPosition(Vector3(-330.0f, 310.0f, 0.0f));
	_MoneyFrame->SetTexture(LOADTEXTURE("UI/Hotbar.png"));
	_MoneyFrame->SetSize(Vector2(310.0f,52.0f));
	_MoneyRender = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("MoneyRender", WindowBackPriorty+ 3);
	_MoneyRender->transform->SetParent(_MoneyFrame->transform);
	_MoneyRender->transform->SetLocalPosition(Vector3(60.0f,-13.0f,0.0f));
	_MoneyRender->SetParamTextPos(_MoneyRender->GetParamTextPos() + Vector3(-380.0f,0.0f,0.0f));


	//�n�߂͔�\��.
	this->SetActive(false, true);
}

/**
* �X�V.
*/
void StatusWindow::Update()
{
	_MoneyRender->SetParam("", "UI/coins.png", INSTANCE(Inventory)->GetPlayerMoney(),fbText::TextAnchorE::UpperLeft);

	bool isChange = false;
	static float ChangeTime = 0.5f;
	static float LocalTime = 0.0f;
	if (XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			if (_NowSelectWindow == 0)
			{
				_NowSelectWindow = _WindowCount - 1;
			}
			else
			{
				_NowSelectWindow -= 1;
			}
			isChange = true;
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			if (_NowSelectWindow == 0)
			{
				_NowSelectWindow = _WindowCount - 1;
			}
			else
			{
				_NowSelectWindow -= 1;
			}
			isChange = true;
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else if (XboxInput(0)->IsPressButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			if (_NowSelectWindow == _WindowCount - 1)
			{
				_NowSelectWindow = 0;
			}
			else
			{
				_NowSelectWindow += 1;
			}
			isChange = true;
		}
		LocalTime += Time::DeltaTime();
		if (LocalTime >= ChangeTime)
		{
			if (_NowSelectWindow == _WindowCount - 1)
			{
				_NowSelectWindow = 0;
			}
			else
			{
				_NowSelectWindow += 1;
			}
			isChange = true;
			LocalTime = 0.0f;
			ChangeTime = 0.01f;
		}
	}
	else
	{
		ChangeTime = 0.5f;
		LocalTime = 0.0f;
	}

	if (isChange)
	{
		for (int i = 0; i < _WindowCount; i++)
		{
			_ItemWindowList[i]->SetActive((i == _NowSelectWindow), true);
		}
		static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"))->DeleteList();
	}

}
