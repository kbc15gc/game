#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryMenu.h"

class Sprite;
class CircleCollision;
class HistoryMenu;

//デバッグ用のボタン
class HistoryMenuButton:public GameObject
{
public:
	HistoryMenuButton(const char* name);
	void Awake()override;
	void Update()override;
	
	void SetChipID(const ChipID& chipid);
	void SetInfo(const int& id,const int& idx)
	{
		_Id = id;
		_Idx = idx;
	}
	void SetMenuButtonFlag(bool flag)
	{
		_MenuButtonFlag = flag;
	}
private:
	void _NextChip();
	void _SetChip();	//セレクトしたチップをセットする
private:
	Sprite* _Sprite;			//スプライト
	CircleCollision* _Circle;	//円のあたり判定
	HistoryMenu* _HistoryMenu;	//歴史メニュー

	ChipID _ChipID;		//歴史チップのID
	TEXTURE* _Textures;	//テクスチャ
	int _Id;
	int _Idx;
	HistoryInfo* _Info;
	
	//TRUE = メニューボタン : FALSE = メニューセレクトボタン
	bool _MenuButtonFlag;
};