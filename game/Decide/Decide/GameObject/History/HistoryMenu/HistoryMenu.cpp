/**
* ���j�ύX���j���[��ʃN���X�̎���.
*/
#include"stdafx.h"
#include "HistoryMenu.h"

#include "fbEngine\_Object\_GameObject\TextObject.h"

#include"..\HistoryBook\HistoryBook.h"

#include "GameObject\History\HistoryManager.h"


/**
* ������.
*/
void HistoryMenu::Start()
{

	_LocationNameRender = INSTANCE(GameObjectManager)->AddNew<TextObject>("LocationNameRender", _Priority);

	_LocationNameRender->Initialize(L"", 80.0f, Color::white, fbSprite::SpriteEffectE::OUTLINE, STRING(fbText::TextStyleE::�l�r_����));
	

	//���W��ݒ�.
	_LocationNameRender->transform->SetLocalPosition(Vector3(g_WindowSize.x / 2.0f, 50.0f, 0));
	//�\������ݒ�.
	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

	//���j���̃|�C���^���擾.
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	_ReleaseLocation = (int)LocationCodeE::Prosperity;
}

/**
* �X�V.
*/
void HistoryMenu::Update()
{
	if (_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Idol)
	{
		//�\��.
		EnableUpdate();
	}
	else
	{
		//��\��.
		_LocationNameRender->SetActive(false);

		for (auto& it : _Chip2DList)
		{
			it->SetActive(false);
		}
	}
}

/**
* �`�b�v��ǉ�.
*/
void HistoryMenu::AddChip(ChipID chipID)
{
	Chip2D* chip2D = INSTANCE(GameObjectManager)->AddNew<Chip2D>("Chip2D", 9);
	chip2D->Start(chipID);
	_Chip2DList.push_back(chip2D);
}

/**
* �\�����̍X�V.
*/
void HistoryMenu::EnableUpdate()
{
	//�\��.
	_LocationNameRender->SetActive(true);

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		//���g���K�[.
		_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
	}
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		//�E�g���K�[.
		_NowSelectLocation = max(0, _NowSelectLocation - 1);
	}

	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		INSTANCE(HistoryManager)->SetHistoryChip((LocationCodeE)_NowSelectLocation, 0, _Chip2DList[_NowSelectChip]->GetChipID());
		auto& it = _Chip2DList.begin();
		_Chip2DList.erase(it + _NowSelectChip);
	}

	for (int i = 0; i < _Chip2DList.size(); i++)
	{
		_Chip2DList[i]->SetActive(true);
		if (i == _NowSelectChip)
		{
			_Chip2DList[i]->SetSize(Chip2D::SizeCodeE::Select);
		}
		else
		{
			_Chip2DList[i]->SetSize(Chip2D::SizeCodeE::NoSelect);
		}

		int len = i - _NowSelectChip;
		float offset = -150.0f;
		Vector3 pos = Vector3((g_WindowSize.x / 2.0f) + (offset * len), g_WindowSize.y - 10.0f, 0.0f);
		_Chip2DList[i]->transform->SetPosition(pos);
	}

}
