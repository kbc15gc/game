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

	//メニューボタンとかテキスト生成
	{
		FOR(continent, CONTINENT_NUM)
		{
			//高さ計算
			float height = interval.y * (continent + 1) + hasi.y / 2;

			//大陸のIDのテキスト
			TextObject* ContinentIDText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ContinentID" ,_Priority);
			wchar_t id[20];
			Support::ToString(continent, id);
			ContinentIDText->Initialize(id, 80.0f, Color::dokaben, fbSprite::SpriteEffectE::NONE, STRING(fbText::TextStyleE::NewDokabenFont));
			ContinentIDText->transform->SetLocalPosition(Vector3(150.0f, height, 0.0f));
			_MenuObjects.push_back(ContinentIDText);

			FOR(history, HISTORY_CHIP_NUM)
			{
				//ボタン
				HistoryMenuButton* b = INSTANCE(GameObjectManager)->AddNew<HistoryMenuButton>("HistoryMenuButton", _Priority);
				_Buttons[continent][history] = b;
				b->SetInfo(continent, history);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}
	}

	_SetMenuEnabel(false);
}

void HistoryMenu::Update()
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
}

void HistoryMenu::_SetMenuEnabel(const bool & enabel)
{
	_MenuEnabel = enabel;
	//メニューのオブジェクトすべてに適応する
	for each (GameObject* o in _MenuObjects)
	{
		o->SetActive(_MenuEnabel);
	}
}

void HistoryMenu::_OpenMenu()
{
	//歴史ボタン全部分ループ
	FOR(conti, CONTINENT_NUM)
	{
		//大陸の歴史情報取得
		HistoryInfo* h = INSTANCE(HistoryManager)->GetHistory(conti);
		FOR(chip, HISTORY_CHIP_NUM)
		{
			//ボタンに読み込んだ情報割り当て。
			_Buttons[conti][chip]->SetChipID(h->Chips[chip]);
		}
	}
}