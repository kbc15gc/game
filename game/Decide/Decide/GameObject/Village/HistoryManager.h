#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Button;

namespace
{
	//各大陸の歴史メダルの状況
	struct History:Noncopyable
	{
		History()
		{
			ContinentID = -1;
			FOR(i, HISTORY_CHIP_NUM)
				IsSetting[i] = -1;
		}
		//大陸のID
		int ContinentID;
		//チップがセットされているか？
		bool IsSetting[HISTORY_CHIP_NUM];
	};
}

//大陸の歴史を管理する
class HistoryManager:public GameObject
{
public:
	HistoryManager(const char* name);
	~HistoryManager();

	void Start()override;
	void Update()override;
private:
	//メニューの有効フラグ設定
	void _SetMenuEnabel(const bool& enabel);
	//メニューを開く処理
	void _OpenMenu();
	//メニューを開いている時の処理
	void _MenuUpdate();
	//歴史の変更を更新
	void _UpdateHistory();
private:
	//各大陸の歴史情報のリスト
	vector<History*> _HistoryList;
	//メニューの有効フラグ
	bool _MenuEnabel;

	//歴史ボタンたち
	Button* _HistoryButtons[CONTINENT_NUM][HISTORY_CHIP_NUM];
	//OKボタン
	Button* _OK;
	//メニューに使ってるオブジェクトたち
	vector<GameObject*> _MenuObjects;
};