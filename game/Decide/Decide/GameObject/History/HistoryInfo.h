/**
* ���j�֌W�̏����L�q.
*/
#pragma once

class HistoryPage;

/**
* ���j�`�b�vID.
*/
enum class ChipID : int
{
	None = -1,	//!< �ݒ�Ȃ�.
	Fire,		//!< �΂̃`�b�v.
	Tree,		//!< �؂̃`�b�v.
	Stone,		//!< �΂̃`�b�v.
	Copper,		//!< ���̃`�b�v.
	Agriculture,//!< �_�Ƃ̃`�b�v
	Hunt,		//!< ���̃`�b�v.
	Iron,		//!< �S�̃`�b�v.
	Oil,		//!< �Ζ��̃`�b�v.
	Medicine,	//!< ��̃`�b�v.
	//DevilTown,	//!< �����̊X�̃`�b�v.
	//DevilCastel,//!< �����̏�̃`�b�v.
	//DevilThrone,//!< �����̋ʍ��̃`�b�v.
	ChipNum,	//!< �`�b�v�̐�.
};

/**
* �`�b�v�̖��O.
*/
static const string ChipName[(int)ChipID::ChipNum] =
{
	"�΂̃`�b�v",
	"�؂̃`�b�v",
	"�΂̃`�b�v",
	"���̃`�b�v",
	"�_�Ƃ̃`�b�v",
	"���̃`�b�v",
	"�S�̃`�b�v",
	"�Ζ��̃`�b�v",
	"��̃`�b�v",
};

/**
* �`�b�v�̃e�N�X�`����
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
* �ꏊ�R�[�h.
*/
enum class LocationCodeE : int
{
	None = -1,
	Begin = 0,			//!< �͂��܂�̏W��.
	Hunting,		//!< ��̑�.
	Prosperity,		//!< �ɉh�̊X.
	Common,			//!< ���ʂ�NPC.
	LocationNum,	//!< �ꏊ�̐�.
};

// �����R�[�h�B
// ���j����̃R�[�h�Ƀo�O���o���炢���Ȃ��̂Ŗ�������܂߂����̗񋓂͂�����ɋL�ڂ���B
enum class LocationCodeAll : int
{
	None = -1,
	Drarian = 0,
	Gremrin,
	BossD,
	UraDorarian,
	UraBossD,
	UraGuremurin,
	Begin,			//!< �͂��܂�̏W��.
	Hunting,		//!< ��̑�.
	Prosperity,		//!< �ɉh�̊X.
	Kuni,			// �M�̍��B
	Sinden,			// �S���̐_�a�B
	DevilKingdom,	//!< ���q�̖���.�������ȍ~�̓}�b�v�ɋL����Ȃ��B
	World,
	Max
};

// �����R�[�h�B
// ���j����̃R�[�h�Ƀo�O���o���炢���Ȃ��̂Ŗ�������܂߂����̖��O�͂�����ɋL�ڂ���B
static const string AllLocationNameList[(int)LocationCodeAll::Max] =
{
	"�h�����A���̏Z��",
	"�O���������̓��A",
	"�w�c�x",
	"���h�����A��",
	"���w�c�x�̗̒n",
	"���O���������̊��",
	"�͂��܂�̏W��",
	"��̑�",
	"�ɉh�̊X",
	"�M�̍�",
	"�S���̐_�a",
	"���q�̖���",
	"",
};

// �X�ɓ��������Ȃǂɏo�����O�B
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
	"����"		// ���g�p�H�B
};

// �����R�[�h�B
// �X�Ȃǂɓ��������ȂǂɃ}�b�v�̒n�����J�����邽�߂̔z��B
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
	LocationCodeAll::Max		// ���g�p�H�B
};

/** 
* �ꏊ��. 
*/
static const string LocationNameList[(int)LocationCodeE::Common] =
{
	AllLocationNameList[(int)LocationCodeAll::Begin],
	AllLocationNameList[(int)LocationCodeAll::Hunting],
	AllLocationNameList[(int)LocationCodeAll::Prosperity],
};

// �����R�[�h�B
// ���j����̃R�[�h�Ƀo�O���o���炢���Ȃ��̂Ŗ�������܂߂����̈ʒu�͂�����ɋL�ڂ���B
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
* �v���C���[�̃��X�|�[���n�_�̍��W.
*/
static const Vector3 LocationPosition[(int)LocationCodeE::Common] =
{
	AllLocationPosition[(int)LocationCodeAll::Begin],
	AllLocationPosition[(int)LocationCodeAll::Hunting],
	AllLocationPosition[(int)LocationCodeAll::Prosperity],
	//Vector3(-114.23f, 183.0f, 184.95f),
};

/**
* �e�ꏊ�̗��j�`�b�v�̏�.
*/
struct LocationHistoryInfo : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
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

	/** �ꏊID. */
	LocationCodeE _LocationID;
	/** �o�^����Ă�����j�`�b�vID���X�g. */
	ChipID _ChipSlot[(int)ChipID::ChipNum];

};

namespace
{

	//�����o�ϐ��̏��ݒ�
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

	//�I�u�W�F�N�g�̏��
	struct VillageGroup
	{
		//�O���[�vID
		int GroupID;
		//�e�X���b�g�̐ݒu��
		ChipID Slot[(int)ChipID::ChipNum];
	};

	//�����o�ϐ��̏��ݒ�
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