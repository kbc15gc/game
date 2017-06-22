/**
* ���j�ύX���j���[��ʃN���X�̎���.
*/
#include"stdafx.h"
#include "HistoryMenu.h"

#include "fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\HistoryBook\HistoryBook.h"

/**
* �������.
*/
namespace
{

	/** �ꏊ��. */
	string LocationNameList[(int)HistoryMenu::LocationCodeE::LocationNum] =
	{
		"�n�܂�̏W��",
		"��̑�",
		"�ɉh�̒�",
	};

}

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
	}
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
		_NowSelectLocation = max(0, _NowSelectLocation - 1);
	}
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		//�E�g���K�[.
		_NowSelectLocation = min(_ReleaseLocation, _NowSelectLocation + 1);
	}

	_LocationNameRender->SetString(LocationNameList[_NowSelectLocation].c_str());

}
