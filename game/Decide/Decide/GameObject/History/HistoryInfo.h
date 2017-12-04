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
	Hunt,		//!< ���̃`�b�v.
	Agriculture,//!< �_�Ƃ̃`�b�v
	Copper,		//!< ���̃`�b�v.
	Iron,		//!< �S�̃`�b�v.
	Oil,		//!< �Ζ��̃`�b�v.
	Medicine,	//!< ��̃`�b�v.
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
	"HuntPage.png",
	"AgriculturePage.png",
	"CopperPage.png",
	"IronPage.png",
	"OilPage.png",
	"MedicinePage.png",
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
	DevilKingdom,	//!< �����̍�.	
	Common,			//!< ���ʂ�NPC.
	LocationNum,	//!< �ꏊ�̐�.
};

/** 
* �ꏊ��. 
*/
static const string LocationNameList[(int)LocationCodeE::Common] =
{
	"�͂��܂�̏W��",
	"��̑�",
	"�ɉh�̊X",
	"�����̍�",
};

/**
* �v���C���[�̃��X�|�[���n�_�̍��W.
*/
static const Vector3 LocationPosition[(int)LocationCodeE::Common] =
{
	Vector3(-387.3f,58.0f,-75.8f),
	Vector3(-108.1f ,55.5f ,533.9f),
	Vector3(212.88f, 67.0f, 6.92f),
	Vector3(-114.23f, 183.0f, 184.95f),
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
	};

}