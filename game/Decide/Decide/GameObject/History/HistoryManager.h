/**
* 歴史を管理するクラスの定義.
*/
#pragma once

#include"HistoryInfo.h"
#include"HistoryMenu\HistoryMenu.h"
#include"HistoryBook\HistoryBook.h"

/** 各場所の歴史チップの状況. */
struct LocationHistoryInfo;

/**
* 歴史を管理するクラス.
*/
class HistoryManager
{
private:

	/**
	* コンストラクタ.
	*/
	HistoryManager();

	/**
	* デストラクタ.
	*/
	~HistoryManager()
	{
	}

public:

	/**
	* インスタンスの取得.
	*/
	static HistoryManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new HistoryManager();
		}
		return _Instance;
	}

	/**
	* 初期化.
	*/
	void Start();
	
	/**
	* 歴史オブジェクト生成.
	*/
	void CreateObject();

	/**
	* 歴史を変える.
	* 
	* @param location	場所ID.
	* @param slot		スロット番号.
	* @param chip		チップID.
	*/
	bool SetHistoryChip(LocationCodeE location, ChipID chip);

	/**
	* 歴史情報を取得.
	*
	* @param location	場所ID.
	*/
	/*LocationHistoryInfo* GetHistory(LocationCodeE location)
	{
		return _LocationHistoryList.at((int)location).get();
	}*/

	/**
	* 新しいチップIDを追加.
	*/
	void AddPossessionChip(ChipID chipID)
	{
		_PossessionChipList.push_back(chipID);
		_HistoryMenu->AddChip(chipID);
	}

	//歴史書から指定されたページを削除
	void PutOutPage(LocationCodeE location, vector<HistoryPage*>& list);

private:
	
	/**
	* 場所の変化.
	*
	* @param location	場所ID.
	*/
	void _ChangeLocation(LocationCodeE location);

	/**
	* パターン計算.
	*
	* @param info	場所のアドレス.
	*/
	int _CalcPattern(const LocationHistoryInfo* info);

	/**
	* オブジェクトを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	*/
	void _CreateObject(int location,const char* path);

	/**
	* コリジョンを作成.
	*
	* @param location	場所ID.
	* @param path		フォルダパス.
	*/
	void _CreateCollision(int location, const char* path);

private:

	/** インスタンス. */
	static HistoryManager* _Instance;
	
	/** 全場所の歴史の情報リスト. */
	vector<unique_ptr<LocationHistoryInfo>> _LocationHistoryList;

	/** 全場所の歴史によって出されたオブジェクトへのポインタリスト. */
	vector<vector<GameObject*>> _GameObjectList;

	/** ヒストリ―メニュークラスのポインタ. */
	HistoryMenu* _HistoryMenu = nullptr;

	/** 歴史書クラスのポインタ. */
	HistoryBook* _HistoryBook = nullptr;

	/** 所持しているチップのID. */
	vector<ChipID> _PossessionChipList;

};