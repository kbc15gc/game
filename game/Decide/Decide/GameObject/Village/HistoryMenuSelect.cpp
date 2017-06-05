#include "stdafx.h"
#include "HistoryMenuSelect.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryButton.h"

HistoryMenuSelect::HistoryMenuSelect(const char * name) :
	GameObject(name)
{

}

HistoryMenuSelect::~HistoryMenuSelect()
{
}

void HistoryMenuSelect::Awake()
{
}

void HistoryMenuSelect::Start()
{
}

void HistoryMenuSelect::Update()
{
	//E�L�[�̉���
	if (KeyBoardInput->isPush(DIK_E))
	{
		//�t���O���]
		_SetMenuEnabel(!_MenuEnabel);
	}

}

void HistoryMenuSelect::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//���j���[�̃I�u�W�F�N�g���ׂĂɓK������
	for each (GameObject* o in _MenuSelectObjects)
	{
		o->SetActive(_MenuEnabel);
	}
}

void HistoryMenuSelect::FireSelect()
{
	//�΂̃{�^��
	HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
	b->SetMenuButtonFlag(false);
	b->SetChipID(ChipID::FIRE);
	Vector3 pos = Vector3(1000.0f, 200.0f, 0.0f);
	b->transform->SetLocalPosition(pos);
	_MenuSelectObjects.push_back(b);

	//�ŏ��͕\�����Ȃ�����FALSE
	_SetMenuEnabel(false);
}

void HistoryMenuSelect::TetuSelect()
{
	//�S�̃{�^��
	HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
	b->SetMenuButtonFlag(false);
	b->SetChipID(ChipID::IRON);
	Vector3 pos = Vector3(1000.0f, 350.0f, 0.0f);
	b->transform->SetLocalPosition(pos);
	_MenuSelectObjects.push_back(b);

	//�ŏ��͕\�����Ȃ�����FALSE
	_SetMenuEnabel(false);
}

void HistoryMenuSelect::AburaSelect()
{
	//���̃{�^��
	HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
	b->SetMenuButtonFlag(false);
	b->SetChipID(ChipID::OIL);
	Vector3 pos = Vector3(1000.0f, 500.0f, 0.0f);
	b->transform->SetLocalPosition(pos);
	_MenuSelectObjects.push_back(b);

	//�ŏ��͕\�����Ȃ�����FALSE
	_SetMenuEnabel(false);
}

void HistoryMenuSelect::MenuSelectNumber()
{
	/*switch (_MenuNomber)
	{
	case MenuNomber::One:
		break;
	
	}*/
}