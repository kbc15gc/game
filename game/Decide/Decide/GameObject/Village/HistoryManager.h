/**
* 歴史を管理するクラスの定義.
*/
#pragma once

struct HistoryInfo;

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
	* 歴史オブジェクト生成.
	*/
	void CreateObject();
	//歴史を変える。
	//[in] unsigned int 変更したい大陸のID
	//[in] unsigned int セットするスロット
	//[in] int セットしたいチップのID
	const bool SetHistoryChip(const unsigned int& continent,const unsigned int& idx, const int& chip);
	//歴史情報を取得
	//[in] unsigned int 歴史を取得したい大陸の添え字
	//[out] HistoryInfo* 歴史情報
	HistoryInfo* GetHistory(const unsigned int idx)
	{
		return _HistoryList.at(idx);
	}
	static HistoryManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new HistoryManager();
		}
		return _Instance;
	}
private:
	//大陸の変化
	void _ChangeContinent(const unsigned int& continent);
	//パターン計算
	const int _CalcPattern(const HistoryInfo* info);

	void _CreateObject(const int& continent,const char* path);
	void _CreateCollision(const int& continent, const char* path);
private:

	/** インスタンス. */
	static HistoryManager* _Instance;
	
	/** 全大陸の歴史の情報リスト. */
	vector<HistoryInfo*> _HistoryList;
	/** 全大陸の歴史によって出されたオブジェクトへのポインタリスト. */
	vector<vector<GameObject*>> _GameObjectList;

};