#include"stdafx.h"
#include "HistoryButton.h"

#include "fbEngine\_Object\_Component\_2D\Sprite.h"
#include "fbEngine\_Object\_Component\_2D\CircleCollision.h"
#include "GameObject\Village\HistoryManager.h"

namespace
{
	//テクスチャファイル名
	char* ChipTexture[(int)ChipID::NUM]
	{
		"nullChip.png",
		"FireChip.png",
		"IronChip.png",
		"OilChip.png",
	};
}

HistoryMenuButton::HistoryMenuButton(const char * name):
	GameObject(name),
	_ChipID(ChipID::NONE),
	_MenuButtonFlag(true)
{
}

void HistoryMenuButton::Awake()
{
	_Sprite = AddComponent<Sprite>();
	_Circle = AddComponent<CircleCollision>();
	_HistoryMenu = (HistoryMenu*)INSTANCE(GameObjectManager)->FindObject("HistoryMenu");
}

void HistoryMenuButton::Update()
{
	if(MouseInput->GetUp(MouseInE::L_CLICK) &&		//左クリック
		_Circle->Judgment(MouseInput->GetCursorPosOnWindow(g_MainWindow)))	//重なっている。
	{
		//メニューフラグがTRUEなら次のチップを設定。
		if (_MenuButtonFlag)
		{
			_NextChip();
		}
		//メニューフラグがFALSEなら選択したチップを設定。
		else
		{
			_SetChip();
		}
		
	}
}

void HistoryMenuButton::SetChipID(const ChipID & chipid)
{
	//ID設定
	_ChipID = chipid;
	//テクスチャ読み込み
	TEXTURE* tex = LOADTEXTURE(ChipTexture[(int)_ChipID]);
	_Sprite->SetTexture(tex);
	//直径
	float len = max(tex->Size.x, tex->Size.y);
	_Circle->SetRadius(len / 2);
}

void HistoryMenuButton::_NextChip()
{
	int id = (int)_ChipID;
	while (true)
	{
		//次へ進める処理
		id = (id + 1) % (int)ChipID::NUM;
		//チップをセットする
		if (INSTANCE(HistoryManager)->SetHistoryChip(_Id, _Idx, (const int)id))
		{
			//セットができたのなら
			SetChipID((ChipID)id);
			break;
		}
	}
}

void HistoryMenuButton::_SetChip()
{
	//チップをセットする
	_HistoryMenu->SetMenuSelectChip(_ChipID);
}