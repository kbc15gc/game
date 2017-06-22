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
			Slot[i] = ChipID::NONE;
	}
	HistoryInfo()
	{
		ContinentID = -1;
		FOR(i, HISTORY_CHIP_NUM)
			Slot[i] = ChipID::NONE;
	}
	//歴史チップをセットする関数。
	//セットする場合は必ずこの関数を使うようにする。
	//[in] unsigned int 何番目にセットするかの添え字
	//[in] ChipID	何のチップをセットするか？
	//[out] bool セットできたかどうか？
	bool SetChip(const unsigned int& idx, const ChipID& chip)
	{
		//範囲内チェック
		if (idx < HISTORY_CHIP_NUM)
		{
			//何かチップを設定する場合。
			if (chip != ChipID::NONE)
			{
				//同じチップを複数つけないようにチェック。
				for each (ChipID Chip in Slot)
				{
					//既につけられていたので終了。
					if (Chip == chip)
						return false;
				}
			}
			//無事設定。
			Slot[idx] = chip;
			return true;
		}
		return false;
	}
public:
	//大陸のID
	int ContinentID;
	//歴史チップのID
	ChipID Slot[HISTORY_CHIP_NUM];
};

namespace
{
	//メンバ変数の情報設定
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT , offsetof(struct HistoryInfo,ContinentID),sizeof(int) },
		{ "Slot[0]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[0]),sizeof(int) },
		{ "Slot[1]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[1]),sizeof(int) },
		{ "Slot[2]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[2]),sizeof(int) }
	};

	//オブジェクトの情報
	struct VillageGroup
	{
		//グループID
		int GroupID;
		//各スロットの設置状況
		ChipID Slot[3];
	};

	//メンバ変数の情報設定
	const Support::DATARECORD VillageGroupData[4] =
	{
		{ "GroupID",Support::DataTypeE::INT, offsetof(struct VillageGroup,GroupID),sizeof(int) },
		{ "Slot[0]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[0]),sizeof(ChipID) },
		{ "Slot[1]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[1]),sizeof(ChipID) },
		{ "Slot[2]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[2]),sizeof(ChipID) },
	};
}