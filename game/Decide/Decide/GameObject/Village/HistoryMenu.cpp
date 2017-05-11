#include "HistoryMenu.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryButton.h"

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
				_Buttons[continent][history] = b;
				b->SetInfo(continent, history);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}
	}

	//OK�{�^��
	_OK = INSTANCE(GameObjectManager)->AddNew<Button>("HistoryMenuButton", _Priority);
	_OK->Initialize(LOADTEXTURE("SAVE.png"), nullptr, LOADTEXTURE("ON.png")->Size, fbButton::ButtonTypeE::PRESS);

	_OK->transform->SetLocalPosition(Vector3(interval.x * (CONTINENT_NUM + 1), interval.y * HISTORY_CHIP_NUM, 0.0));
	_MenuObjects.push_back(_OK);

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

	//���j���[���J����Ă���
	if (_MenuEnabel)
	{
		//�X�V
		_MenuUpdate();
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
}

void HistoryMenu::_OpenMenu()
{
	//���j���擾

	//���j�{�^���S�������[�v
	FOR(conti, CONTINENT_NUM)
	{
		HistoryInfo* h = INSTANCE(HistoryManager)->GetHistory(conti);
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//�{�^���ɓǂݍ��񂾏�񊄂蓖�āB
			_Buttons[conti][chip]->SetChipID(h->Chips[chip]);
		}
	}
}

void HistoryMenu::_MenuUpdate()
{
	//OK�{�^���������ꂽ
	if(_OK->GetIsOn())
	{
		//�{�^���̏󋵂𔽉f
		_UpdateHistory();
	}
}

void HistoryMenu::_UpdateHistory()
{
	//�{�^���̏󋵂���j�ɔ��f
	FOR(conti, CONTINENT_NUM)
	{
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//INSTANCE(HistoryManager)->SetHistoryChip(conti, chip, (ChipID)-1);
		}
	}
}