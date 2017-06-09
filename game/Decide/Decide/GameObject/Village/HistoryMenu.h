#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\HistoryInfo.h"

class Button;
class HistoryMenuButton;

//歴史変更のデバッグメニュー
class HistoryMenu:public GameObject
{
public:
	enum MenuNomber
	{
		One = 0,
		Two,
		Three,
	};
	HistoryMenu(const char* name);
	~HistoryMenu();

	void Start()override;
	void Update()override;

	//選択したチップをメニューに設定する。
	void SetMenuSelectChip(ChipID chipid);
private:
	//メニューの有効フラグ設定
	void _SetMenuEnabel(const bool& enabel);
	//メニューを開く処理
	void _OpenMenu();
	//セレクトボタンの更新(チップをセットする場所を決める)
	void SelectMenuButton();
private:
	//メニューの有効フラグ
	bool _MenuEnabel;

	//ボタンたち
	HistoryMenuButton* _Buttons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OKボタン
	Button* _OK;
	//メニューに使ってるオブジェクトたち
	vector<GameObject*> _MenuObjects;
	//選択ボタンイメージ
	ImageObject* _SelectImage;
	//メニューで何番を選択するか。
	MenuNomber _MenuNomber;
};