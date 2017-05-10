#include "HistoryManager.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

namespace
{
	//�����o�ϐ��̏��ݒ�
	Support::DATARECORD Historydata[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT , offsetof(struct History,ContinentID),sizeof(int) },
		{ "IsSetting[0]",Support::DataTypeE::INT, offsetof(struct History,IsSetting[0]),sizeof(bool) },
		{ "IsSetting[1]",Support::DataTypeE::INT, offsetof(struct History,IsSetting[1]),sizeof(bool) },
		{ "IsSetting[2]",Support::DataTypeE::INT, offsetof(struct History,IsSetting[2]),sizeof(bool) }
	};
}

HistoryManager::HistoryManager(const char * name) :
	GameObject(name)
{

}

HistoryManager::~HistoryManager()
{
}

void HistoryManager::Start()
{
	Vector2 hasi(250, 200);
	Vector2 interval(150, 100);
	int priority = 10;

	//���j���[�{�^���Ƃ��e�L�X�g����
	{
		FOR(continent, CONTINENT_NUM)
		{
			//�嗤��ID�̃e�L�X�g
			TextObject* ID = INSTANCE(GameObjectManager)->AddNew<TextObject>("ID", priority);
			wchar_t id[20];
			Support::ToString(continent, id);
			ID->Initialize(id, 80.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			float height = interval.y * (continent + 1) + hasi.y / 2;
			ID->transform->SetLocalPosition(Vector3(150.0f, height, 0.0f));
			_MenuObjects.push_back(ID);

			FOR(history, HISTORY_CHIP_NUM)
			{
				//���j��ON�EOFF�{�^��
				Button* b = _HistoryButtons[continent][history] = INSTANCE(GameObjectManager)->AddNew<Button>("HistoryMenuButton", priority);
				b->Initialize(LOADTEXTURE("ON.png"), LOADTEXTURE("OFF.png"), LOADTEXTURE("ON.png")->Size, fbButton::ButtonTypeE::TRIGGER);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}

		wchar_t* chipnames[HISTORY_CHIP_NUM] = { L"�q",L"�e�c",L"�Z�L��" };

		FOR(history, HISTORY_CHIP_NUM)
		{
			//���j�̖��O�e�L�X�g
			TextObject* chip = INSTANCE(GameObjectManager)->AddNew<TextObject>("chip", priority);
			chip->Initialize(chipnames[history], 50.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			float width = interval.x * (history + 1) + hasi.x / 2;
			chip->transform->SetLocalPosition(Vector3(width, 120.0f, 0.0f));
			_MenuObjects.push_back(chip);
		}

		//OK�{�^��
		_OK = INSTANCE(GameObjectManager)->AddNew<Button>("HistoryMenuButton", priority);
		_OK->Initialize(LOADTEXTURE("SAVE.png"), nullptr, LOADTEXTURE("ON.png")->Size, fbButton::ButtonTypeE::PRESS);

		_OK->transform->SetLocalPosition(Vector3(interval.x * (CONTINENT_NUM+1), interval.y * HISTORY_CHIP_NUM, 0.0));
		_MenuObjects.push_back(_OK);

		_SetMenuEnabel(false);
	}
}

void HistoryManager::Update()
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

void HistoryManager::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//���j���[�̃I�u�W�F�N�g���ׂĂɓK������
	for each (GameObject* o in _MenuObjects)
	{
		o->SetActive(_MenuEnabel);
	}
}

void HistoryManager::_OpenMenu()
{
	//�t�@�C���p�X
	const char* filepath = "Asset/Data/VillageHistory.csv";
	FOR(i,_HistoryList.size())
	{
		//�������[���[�N��h��
		SAFE_DELETE(_HistoryList[i]);
	}
	_HistoryList.clear();
	//�t�@�C��������j�̏��ǂݍ���
	Support::LoadCSVData<History>(filepath, Historydata, ARRAY_SIZE(Historydata), _HistoryList);

	//���j�{�^���S�������[�v
	FOR(conti, CONTINENT_NUM)
	{
		History* h = _HistoryList[conti];
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//�{�^���ɓǂݍ��񂾏�񊄂蓖�āB
			_HistoryButtons[h->ContinentID][chip]->SetOnOff(h->IsSetting[chip]);
		}
	}
}

void HistoryManager::_MenuUpdate()
{
	//OK�{�^���������ꂽ
	if(_OK->GetIsOn())
	{
		//�{�^���̏󋵂𔽉f
		_UpdateHistory();
		//�t�@�C���p�X
		const char* filepath = "Asset/Data/VillageHistory.csv";
		//�t�@�C���֏����o��
		Support::OutputCSV<History>(filepath, Historydata, ARRAY_SIZE(Historydata), _HistoryList);
	}
}

void HistoryManager::_UpdateHistory()
{
	//�{�^���̏󋵂���j�ɔ��f
	FOR(conti, CONTINENT_NUM)
	{
		History* h = _HistoryList[conti];
		FOR(chip, HISTORY_CHIP_NUM)
		{
			h->IsSetting[chip] = _HistoryButtons[h->ContinentID][chip]->GetIsOn();
		}
	}
}