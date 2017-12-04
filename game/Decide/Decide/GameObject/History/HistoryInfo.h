/**
* 歴史関係の情報を記述.
*/
#pragma once

class HistoryPage;

/**
* 歴史チップID.
*/
enum class ChipID : int
{
	None = -1,	//!< 設定なし.
	Fire,		//!< 火のチップ.
	Tree,		//!< 木のチップ.
	Stone,		//!< 石のチップ.
	Hunt,		//!< 狩りのチップ.
	Agriculture,//!< 農業のチップ
	Copper,		//!< 銅のチップ.
	Iron,		//!< 鉄のチップ.
	Oil,		//!< 石油のチップ.
	Medicine,	//!< 薬のチップ.
	ChipNum,	//!< チップの数.
};

/**
* チップの名前.
*/
static const string ChipName[(int)ChipID::ChipNum] =
{
	"火のチップ",
	"木のチップ",
	"石のチップ",
	"狩りのチップ",
	"農業のチップ",
	"銅のチップ",
	"鉄のチップ",
	"石油のチップ",
	"薬のチップ",
};

/**
* チップのテクスチャ名
*/
static const string ChipFileName[(int)ChipID::ChipNum] = 
{
	"FirePage.png",
	"TreePage.png",
	"StonePage.png",
	"HuntPage.png",
	"AgriculturePage.png",
	"CopperPage.png",
	"IronPage.png",
	"OilPage.png",
	"MedicinePage.png",
};

/**
* 場所コード.
*/
enum class LocationCodeE : int
{
	None = -1,
	Begin = 0,			//!< はじまりの集落.
	Hunting,		//!< 狩猟の村.
	Prosperity,		//!< 繁栄の街.
	DevilKingdom,	//!< 魔王の国.	
	Common,			//!< 共通のNPC.
	LocationNum,	//!< 場所の数.
};

/** 
* 場所名. 
*/
static const string LocationNameList[(int)LocationCodeE::Common] =
{
	"はじまりの集落",
	"狩猟の村",
	"繁栄の街",
	"魔王の国",
};

/**
* プレイヤーのリスポーン地点の座標.
*/
static const Vector3 LocationPosition[(int)LocationCodeE::Common] =
{
	Vector3(-387.3f,58.0f,-75.8f),
	Vector3(-108.1f ,55.5f ,533.9f),
	Vector3(212.88f, 67.0f, 6.92f),
	Vector3(-114.23f, 183.0f, 184.95f),
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

	void SetData(vector<HistoryPage*>& list);


public:

	/** 場所ID. */
	LocationCodeE _LocationID;
	/** 登録されている歴史チップIDリスト. */
	ChipID _ChipSlot[(int)ChipID::ChipNum];

};

namespace
{

	//メンバ変数の情報設定
	const Support::DATARECORD HistoryInfoData[] =
	{
		{ "ContinentID",Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_LocationID),	sizeof(int) },
		{ "Slot[0]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[0]),	sizeof(int) },
		{ "Slot[1]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[1]),	sizeof(int) },
		{ "Slot[2]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[2]),	sizeof(int) },
		{ "Slot[3]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[3]),	sizeof(int) },
		{ "Slot[4]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[4]),	sizeof(int) },
		{ "Slot[5]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[5]),	sizeof(int) },
		{ "Slot[6]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[6]),	sizeof(int) },
		{ "Slot[7]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[7]),	sizeof(int) },
		{ "Slot[8]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[8]),	sizeof(int) },
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
	const Support::DATARECORD VillageGroupData[] =
	{
		{ "GroupID",Support::DataTypeE::INT, offsetof(struct VillageGroup,GroupID),sizeof(int) },
		{ "Slot[0]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[0]),sizeof(ChipID) },
		{ "Slot[1]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[1]),sizeof(ChipID) },
		{ "Slot[2]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[2]),sizeof(ChipID) },
		{ "Slot[3]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[3]),sizeof(ChipID) },
		{ "Slot[4]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[4]),sizeof(ChipID) },
		{ "Slot[5]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[5]),sizeof(ChipID) },
		{ "Slot[6]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[6]),sizeof(ChipID) },
		{ "Slot[7]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[7]),sizeof(ChipID) },
		{ "Slot[8]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[8]),sizeof(ChipID) },
	};

}