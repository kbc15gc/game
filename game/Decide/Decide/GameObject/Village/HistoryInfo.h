#pragma once


	//歴史チップの種類
enum class ChipID : int
{
	NONE,	//外す
	FIRE,	//火のチップ
	IRON,	//鉄のチップ
	OIL,	//石油のチップ？
	NUM,
};

//各大陸の歴史メダルの状況
struct HistoryInfo :Noncopyable
{
public:
	HistoryInfo(const unsigned int continent)
	{
		ContinentID = continent;
		FOR(i, HISTORY_CHIP_NUM)
			Chips[i] = ChipID::NONE;
		GroupID = 0;
	}
	HistoryInfo()
	{
		ContinentID = -1;
		FOR(i, HISTORY_CHIP_NUM)
			Chips[i] = ChipID::NONE;
		GroupID = 0;
	}
	//歴史チップをセットする関数。
	//セットする場合は必ずこの関数を使うようにする。
	//[in] unsigned int 何番目にセットするかの添え字
	//[in] ChipID	何のチップをセットするか？
	//[out] bool セットできたかどうか？
	bool SetChip(const unsigned int& idx,const ChipID& chip)
	{
		//範囲内チェック
		if (idx < HISTORY_CHIP_NUM)
		{
			//何かチップを設定する場合。
			if (chip != ChipID::NONE)
			{
				//同じチップを複数つけないようにチェック。
				for each (ChipID Chip in Chips)
				{
					//既につけられていたので終了。
					if (Chip == chip)
						return false;
				}
			}
			//無事設定。
			Chips[idx] = chip;
			return true;
		}
		return false;
	}
public:
	//大陸のID
	int ContinentID;
	//歴史チップのID
	ChipID Chips[HISTORY_CHIP_NUM];
	//歴史グループ
	int GroupID;
};

namespace
{
	//メンバ変数の情報設定
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT , offsetof(struct HistoryInfo,ContinentID),sizeof(int) },
		{ "Chips[0]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[0]),sizeof(int) },
		{ "Chips[1]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[1]),sizeof(int) },
		{ "Chips[2]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[2]),sizeof(int) }
	};
}