#pragma once

struct HistoryInfo;

//大陸の歴史を管理するクラス
class HistoryManager
{
public:
	HistoryManager();
	//歴史を変える。
	//[in] unsigned int 変更したい大陸のID
	//[in] unsigned int セットする場所
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
private:
	//全大陸の歴史の情報
	vector<HistoryInfo*> _HistoryList;
	//全大陸の歴史によって出されたオブジェクトへのポインタ
	vector<vector<GameObject*>> _GameObjects;
	static HistoryManager* _Instance;
};