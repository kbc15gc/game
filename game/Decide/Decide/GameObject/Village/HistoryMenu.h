#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Button;
class HistoryMenuButton;

//歴史変更のデバッグメニュー
class HistoryMenu:public GameObject
{
public:
	HistoryMenu(const char* name);
	~HistoryMenu();

	void Start()override;
	void Update()override;
private:
	//メニューの有効フラグ設定
	void _SetMenuEnabel(const bool& enabel);
	//メニューを開く処理
	void _OpenMenu();
private:
	//メニューの有効フラグ
	bool _MenuEnabel;

	//ボタンたち
	HistoryMenuButton* _Buttons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OKボタン
	Button* _OK;
	//メニューに使ってるオブジェクトたち
	vector<GameObject*> _MenuObjects;
};