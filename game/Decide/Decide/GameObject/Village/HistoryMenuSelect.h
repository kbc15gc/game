#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Button;
class HistoryMenuButton;

//歴史変更のデバッグメニュー
class HistoryMenuSelect :public GameObject
{
	
public:
	HistoryMenuSelect(const char* name);
	~HistoryMenuSelect();
	void Awake()override;
	void Start()override;
	void Update()override;
	//どのチップをメニューにセレクトするか。
	void ChipSelect(ChipID id);
private:
	//メニューの有効フラグ設定
	void _SetMenuEnabel(const bool& enabel);
private:
	//メニューの有効フラグ
	bool _MenuEnabel;		
	//ボタンたち
	HistoryMenuButton* _Buttons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OKボタン
	Button* _OK;
	//メニューに使ってるオブジェクトたち
	vector<GameObject*> _MenuSelectObjects;
	GameObject* _MenuSelectFire;
	GameObject* _MenuSelectTetu;
	GameObject* _MenuSelectAbura;
};