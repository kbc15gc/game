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
	Copper,		//!< 銅のチップ.
	Agriculture,//!< 農業のチップ
	Hunt,		//!< 狩りのチップ.
	Iron,		//!< 鉄のチップ.
	Oil,		//!< 石油のチップ.
	Medicine,	//!< 薬のチップ.
	//DevilTown,	//!< 魔王の街のチップ.
	//DevilCastel,//!< 魔王の城のチップ.
	//DevilThrone,//!< 魔王の玉座のチップ.
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
	"銅のチップ",
	"農業のチップ",
	"狩りのチップ",
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
	"CopperPage.png",
	"AgriculturePage.png",
	"HuntPage.png",
	"IronPage.png",
	"OilPage.png",
	"MedicinePage.png",
	//"None.png",
	//"None.png",
	//"None.png",
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
	Common,			//!< 共通のNPC.
	LocationNum,	//!< 場所の数.
};

// くそコード。
// 歴史周りのコードにバグが出たらいけないので魔王城を含めた町の列挙はこちらに記載する。
enum class LocationCodeAll : int
{
	None = -1,
	Drarian = 0,
	Gremrin,
	BossD,
	UraDorarian,
	UraBossD,
	UraGuremurin,
	Begin,			//!< はじまりの集落.
	Hunting,		//!< 狩猟の村.
	Prosperity,		//!< 繁栄の街.
	Kuni,			// 信仰の国。
	Sinden,			// 心酔の神殿。
	DevilKingdom,	//!< 崇拝の魔城.※ここ以降はマップに記されない。
	World,
	Max
};

// くそコード。
// 歴史周りのコードにバグが出たらいけないので魔王城を含めた町の名前はこちらに記載する。
static const string AllLocationNameList[(int)LocationCodeAll::Max] =
{
	"ドラリアンの住処",
	"グレムリンの洞窟",
	"『Ｄ』",
	"裏ドラリアン",
	"裏『Ｄ』の領地",
	"裏グレムリンの岩場",
	"はじまりの集落",
	"狩猟の村",
	"繁栄の街",
	"信仰の国",
	"心酔の神殿",
	"崇拝の魔城",
	"",
};

// 街に入った時などに出す名前。
static const string LocationNames[] =
{
	AllLocationNameList[static_cast<int>(LocationCodeAll::Drarian)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Gremrin)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::BossD)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::UraDorarian)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::UraBossD)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::UraGuremurin)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Begin)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Hunting)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Prosperity)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Kuni)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::DevilKingdom)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Sinden)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::World)],
	"そと"		// 未使用？。
};

// くそコード。
// 街などに入った時などにマップの地名を開放するための配列。
static const LocationCodeAll LocationMapCode[] =
{
	LocationCodeAll::Drarian,
	LocationCodeAll::Gremrin,
	LocationCodeAll::BossD,
	LocationCodeAll::UraDorarian,
	LocationCodeAll::UraBossD,
	LocationCodeAll::UraGuremurin,
	LocationCodeAll::Begin,
	LocationCodeAll::Hunting,
	LocationCodeAll::Prosperity,
	LocationCodeAll::Kuni,
	LocationCodeAll::DevilKingdom,
	LocationCodeAll::Sinden,
	LocationCodeAll::World,
	LocationCodeAll::Max		// 未使用？。
};

/** 
* 場所名. 
*/
static const string LocationNameList[(int)LocationCodeE::Common] =
{
	AllLocationNameList[(int)LocationCodeAll::Begin],
	AllLocationNameList[(int)LocationCodeAll::Hunting],
	AllLocationNameList[(int)LocationCodeAll::Prosperity],
};

// くそコード。
// 歴史周りのコードにバグが出たらいけないので魔王城を含めた町の位置はこちらに記載する。
static const Vector3 AllLocationPosition[(int)LocationCodeAll::Max] =
{
	Vector3(-683.3f,61.7f,63.2f),
	Vector3(308.7f,55.4f,623.2f),
	Vector3(456.7f,71.18f,75.7f),
	Vector3(-651.6f,79.0f,554.23f),
	Vector3(-636.1f,57.7f,-658.5f),
	Vector3(367.17f,87.71f,-390.67f),
	Vector3(-387.3f,58.307f,-75.8f),
	Vector3(-108.1f ,55.524f ,533.9f),
	Vector3(218.88f, 65.793f, -0.92f),
	Vector3(-145.6f, 121.8f, 131.6f),
	Vector3(-145.6f, 188.5f, 239.24f),
	Vector3(0.0f,0.0f,0.0f),
	Vector3(0.0f,0.0f,0.0f),
};


/**
* プレイヤーのリスポーン地点の座標.
*/
static const Vector3 LocationPosition[(int)LocationCodeE::Common] =
{
	AllLocationPosition[(int)LocationCodeAll::Begin],
	AllLocationPosition[(int)LocationCodeAll::Hunting],
	AllLocationPosition[(int)LocationCodeAll::Prosperity],
	//Vector3(-114.23f, 183.0f, 184.95f),
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
		/*{ "Slot[9]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[9]),	sizeof(int) },
		{ "Slot[10]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[10]),	sizeof(int) },
		{ "Slot[11]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[11]),	sizeof(int) },*/
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
		/*{ "Slot[9]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[9]),sizeof(ChipID) },
		{ "Slot[10]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[10]),sizeof(ChipID) },
		{ "Slot[11]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[11]),sizeof(ChipID) },*/
	};

}