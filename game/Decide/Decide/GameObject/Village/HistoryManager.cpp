#include "HistoryManager.h"
#include "fbEngine\_Object\_GameObject\Button.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

namespace
{
	//メンバ変数の情報設定
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

	//メニューボタンとかテキスト生成
	{
		FOR(continent, CONTINENT_NUM)
		{
			//大陸のIDのテキスト
			TextObject* ID = INSTANCE(GameObjectManager)->AddNew<TextObject>("ID", priority);
			wchar_t id[20];
			Support::ToString(continent, id);
			ID->Initialize(id, 80.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			float height = interval.y * (continent + 1) + hasi.y / 2;
			ID->transform->SetLocalPosition(Vector3(150.0f, height, 0.0f));
			_MenuObjects.push_back(ID);

			FOR(history, HISTORY_CHIP_NUM)
			{
				//歴史のON・OFFボタン
				Button* b = _HistoryButtons[continent][history] = INSTANCE(GameObjectManager)->AddNew<Button>("HistoryMenuButton", priority);
				b->Initialize(LOADTEXTURE("ON.png"), LOADTEXTURE("OFF.png"), LOADTEXTURE("ON.png")->Size, fbButton::ButtonTypeE::TRIGGER);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}

		wchar_t* chipnames[HISTORY_CHIP_NUM] = { L"ヒ",L"テツ",L"セキユ" };

		FOR(history, HISTORY_CHIP_NUM)
		{
			//歴史の名前テキスト
			TextObject* chip = INSTANCE(GameObjectManager)->AddNew<TextObject>("chip", priority);
			chip->Initialize(chipnames[history], 50.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			float width = interval.x * (history + 1) + hasi.x / 2;
			chip->transform->SetLocalPosition(Vector3(width, 120.0f, 0.0f));
			_MenuObjects.push_back(chip);
		}

		//OKボタン
		_OK = INSTANCE(GameObjectManager)->AddNew<Button>("HistoryMenuButton", priority);
		_OK->Initialize(LOADTEXTURE("SAVE.png"), nullptr, LOADTEXTURE("ON.png")->Size, fbButton::ButtonTypeE::PRESS);

		_OK->transform->SetLocalPosition(Vector3(interval.x * (CONTINENT_NUM+1), interval.y * HISTORY_CHIP_NUM, 0.0));
		_MenuObjects.push_back(_OK);

		_SetMenuEnabel(false);
	}
}

void HistoryManager::Update()
{
	//Eキーの押下
	if (KeyBoardInput->isPush(DIK_E))
	{
		//フラグ反転
		_SetMenuEnabel(!_MenuEnabel);
		if (_MenuEnabel)
		{
			//メニューを開く
			_OpenMenu();
		}
	}

	//メニューが開かれている
	if (_MenuEnabel)
	{
		//更新
		_MenuUpdate();
	}
}

void HistoryManager::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//メニューのオブジェクトすべてに適応する
	for each (GameObject* o in _MenuObjects)
	{
		o->SetActive(_MenuEnabel);
	}
}

void HistoryManager::_OpenMenu()
{
	//ファイルパス
	const char* filepath = "Asset/Data/VillageHistory.csv";
	FOR(i,_HistoryList.size())
	{
		//メモリーリークを防ぐ
		SAFE_DELETE(_HistoryList[i]);
	}
	_HistoryList.clear();
	//ファイルから歴史の情報読み込み
	Support::LoadCSVData<History>(filepath, Historydata, ARRAY_SIZE(Historydata), _HistoryList);

	//歴史ボタン全部分ループ
	FOR(conti, CONTINENT_NUM)
	{
		History* h = _HistoryList[conti];
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//ボタンに読み込んだ情報割り当て。
			_HistoryButtons[h->ContinentID][chip]->SetOnOff(h->IsSetting[chip]);
		}
	}
}

void HistoryManager::_MenuUpdate()
{
	//OKボタンが押された
	if(_OK->GetIsOn())
	{
		//ボタンの状況を反映
		_UpdateHistory();
		//ファイルパス
		const char* filepath = "Asset/Data/VillageHistory.csv";
		//ファイルへ書き出し
		Support::OutputCSV<History>(filepath, Historydata, ARRAY_SIZE(Historydata), _HistoryList);
	}
}

void HistoryManager::_UpdateHistory()
{
	//ボタンの状況を歴史に反映
	FOR(conti, CONTINENT_NUM)
	{
		History* h = _HistoryList[conti];
		FOR(chip, HISTORY_CHIP_NUM)
		{
			h->IsSetting[chip] = _HistoryButtons[h->ContinentID][chip]->GetIsOn();
		}
	}
}