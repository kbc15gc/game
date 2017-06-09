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
				b->SetMenuButtonFlag(true);//メニューボタンだよ。
				_Buttons[continent][history] = b;
				b->SetInfo(continent, history);
				Vector3 pos = Vector3(interval.x * (history + 1) + hasi.x / 2, height, 0.0f);
				b->transform->SetLocalPosition(pos);
				_MenuObjects.push_back(b);
			}
		}
		//選択ボタン
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
	//メニューを開いているときの処理。
	if (_MenuEnabel)
	{
		//チップをセットする場所を設定する。
		SelectMenuButton();
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
	_SelectImage->SetActive(_MenuEnabel);
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
			_Buttons[conti][chip]->SetChipID(h->Slot[chip]);
		}
	}
}

void HistoryMenu::SelectMenuButton()
{
	int select = (int)_MenuNomber;
	//次のページへ。
	if (KeyBoardInput->isPush(DIK_RIGHT) && _MenuNomber != MenuNomber::Three)
	{
		select++;
	}
	//前のページへ。
	else if (KeyBoardInput->isPush(DIK_LEFT) && _MenuNomber != MenuNomber::One)
	{
		select--;
	}
	//移動した分を渡す。
	_MenuNomber = (MenuNomber)select;

	//各メニューナンバーの座標。
	switch (_MenuNomber)
	{
		//1番目
	case MenuNomber::One :
		_SelectImage->transform->SetLocalPosition(Chip1);
		break;
		//2番目
	case MenuNomber::Two:
		_SelectImage->transform->SetLocalPosition(Chip2);
		break;
		//3番目
	case MenuNomber::Three:
		_SelectImage->transform->SetLocalPosition(Chip3);
		break;
	default:
		break;
	}
}

void HistoryMenu::SetMenuSelectChip(ChipID chipid)
{
	static int chip = 0;		//チップ
	static int continent = 0;	//大陸
	
	//ボタンに読み込んだ情報割り当て。
	//セットできたなら。
	if (INSTANCE(HistoryManager)->SetHistoryChip(continent, _MenuNomber, (const int)chipid))
	{
		_Buttons[continent][_MenuNomber]->SetChipID(chipid);
	}
}
