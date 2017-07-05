/**
* 歴史関係の情報を記述.
*/
#pragma once

/**
* 歴史チップID.
*/
enum class ChipID : int
{
	None = -1,	//!< 設定なし.
	Fire,		//!< 火のチップ.
	Iron,		//!< 鉄のチップ.
	Oil,		//!< 石油のチップ.
	ChipNum,	//!< チップの数.
};

/**
* チップの名前.
*/
static const string ChipName[(int)ChipID::ChipNum] =
{
	"火のチップ",
	"鉄のチップ",
	"石油のチップ",
};

/**
* チップのテクスチャ名
*/
static const string ChipFileName[(int)ChipID::ChipNum] = 
{
	"FirePage.png",
	"IronPage.png",
	"OilPage.png",
};

/**
* 場所コード.
*/
enum class LocationCodeE : int
{
	Begin = 0,		//!< はじまりの集落.
	Hunting,		//!< 狩猟の村.
	Prosperity,		//!< 繁栄の町.
	LocationNum,	//!< 場所の数.
};

/** 
* 場所名. 
*/
static const string LocationNameList[(int)LocationCodeE::LocationNum] =
{
	"はじまりの集落",
	"狩猟の村",
	"繁栄の町",
};

/**
* 各場所の歴史チップの状況.
*/
struct LocationHistoryInfo : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	LocationHistoryInfo()
	{
		for (int i = 0; i < (int)ChipID::ChipNum; i++)
		{
			_ChipSlot[i] = ChipID::None;
		}
	}

	bool SetChip(ChipID chipID, int slot)
	{
		if (_ChipSlot[slot] == ChipID::None)
		{
			_ChipSlot[slot] = chipID;
		}
		else
		{
			//後ろにずらす.
			//明らかにバグ.
			_ChipSlot[slot + 1] = _ChipSlot[slot];
			_ChipSlot[slot] = chipID;
		}
		return 0;
	}

public:

	/** 場所ID. */
	LocationCodeE _LocationID;
	/** 登録されている歴史チップIDリスト. */
	ChipID _ChipSlot[(int)ChipID::ChipNum];

};

namespace
{

	//メンバ変数の情報設定
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_LocationID),	sizeof(int) },
		{ "Slot[0]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[0]),	sizeof(int) },
		{ "Slot[1]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[1]),	sizeof(int) },
		{ "Slot[2]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[2]),	sizeof(int) }
	};

	//オブジェクトの情報
	struct VillageGroup
	{
		//グループID
		int GroupID;
		//各スロットの設置状況
		ChipID Slot[(int)ChipID::ChipNum];
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