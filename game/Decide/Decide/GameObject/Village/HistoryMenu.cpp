#include"stdafx.h"
#include "HistoryMenu.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryButton.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

namespace
{
	Vector3 Chip1 = Vector3(250, 200, 0.0f);
	Vector3 Chip2 = Vector3(400, 200, 0.0f);
	Vector3 Chip3 = Vector3(550, 200, 0.0f);
}

HistoryMenu::HistoryMenu(const char * name) :
	GameObject(name)
{

}

HistoryMenu::~HistoryMenu()
{
}

void HistoryMenu::Start()
{
	Vector2 hasi(250, 200);
	Vector2 interval(150, 100);
	//���j���[�{�^���Ƃ��e�L�X�g����
	{
		FOR(continent, CONTINENT_NUM)
		{
			//�����v�Z
			float height = interval.y * (continent + 1) + hasi.y / 2;

			//�嗤��ID�̃e�L�X�g
			TextObject* ContinentIDText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ContinentID" ,_Priority);
			wchar_t id[20];
			Support::ToString(continent, id);
			ContinentIDText->Initialize(id, 80.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			ContinentIDText->transform->SetLocalPosition(Vector3(150.0f, height, 0.0f));
			_MenuObjects.push_back(ContinentIDText);

			FOR(history, HISTORY_CHIP_NUM)
			{
				//�{�^��
				HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
				b->SetMenuButtonFlag(true);//���j���[�{�^������B
				_Buttons[continent][history] = b;
				b->SetInfo(continent, history);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}
		//�I���{�^��
		_SelectImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage", _Priority);
		_SelectImage->SetTexture(LOADTEXTURE("cursor.png"));
		_SelectImage->SetPivot(0.5, 1.0);
		_SelectImage->transform->SetLocalPosition(Vector3(hasi.x, hasi.y, 0.0f));
		_SelectImage->transform->SetScale(Vector3::one);
		_SelectImage->SetActive(false);
		_MenuNomber = MenuNomber::One;
	}
	_SetMenuEnabel(false);
}

void HistoryMenu::Update()
{
	//E�L�[�̉���
	if (KeyBoardInput->isPush(DIK_E))
	{
		//�t���O���]
		_SetMenuEnabel(!_MenuEnabel);
		if (_MenuEnabel)
		{
			//���j���[���J��
			_OpenMenu();
		}
	}
	//���j���[���J���Ă���Ƃ��̏����B
	if (_MenuEnabel)
	{
		//�`�b�v���Z�b�g����ꏊ��ݒ肷��B
		SelectMenuButton();
	}
	
}

void HistoryMenu::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//���j���[�̃I�u�W�F�N�g���ׂĂɓK������
	for each (GameObject* o in _MenuObjects)
	{
		o->SetActive(_MenuEnabel);
	}
	_SelectImage->SetActive(_MenuEnabel);
}

void HistoryMenu::_OpenMenu()
{
	//���j�{�^���S�������[�v
	FOR(conti, CONTINENT_NUM)
	{
		//�嗤�̗��j���擾
		HistoryInfo* h = INSTANCE(HistoryManager)->GetHistory(conti);
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//�{�^���ɓǂݍ��񂾏�񊄂蓖�āB
			_Buttons[conti][chip]->SetChipID(h->Slot[chip]);
		}
	}
}

void HistoryMenu::SelectMenuButton()
{
	int select = (int)_MenuNomber;
	//���̃y�[�W�ցB
	if (KeyBoardInput->isPush(DIK_RIGHT) && _MenuNomber != MenuNomber::Three)
	{
		select++;
	}
	//�O�̃y�[�W�ցB
	else if (KeyBoardInput->isPush(DIK_LEFT) && _MenuNomber != MenuNomber::One)
	{
		select--;
	}
	//�ړ���������n���B
	_MenuNomber = (MenuNomber)select;

	//�e���j���[�i���o�[�̍��W�B
	switch (_MenuNomber)
	{
		//1�Ԗ�
	case MenuNomber::One :
		_SelectImage->transform->SetLocalPosition(Chip1);
		break;
		//2�Ԗ�
	case MenuNomber::Two:
		_SelectImage->transform->SetLocalPosition(Chip2);
		break;
		//3�Ԗ�
	case MenuNomber::Three:
		_SelectImage->transform->SetLocalPosition(Chip3);
		break;
	default:
		break;
	}
}

void HistoryMenu::SetMenuSelectChip(ChipID chipid)
{
	static int chip = 0;		//�`�b�v
	static int continent = 0;	//�嗤
	
	//�{�^���ɓǂݍ��񂾏�񊄂蓖�āB
	//�Z�b�g�ł����Ȃ�B
	if (INSTANCE(HistoryManager)->SetHistoryChip(continent, _MenuNomber, (const int)chipid))
	{
		_Buttons[continent][_MenuNomber]->SetChipID(chipid);
	}
}
