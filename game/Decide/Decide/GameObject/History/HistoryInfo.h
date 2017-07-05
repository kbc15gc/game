/**
* ���j�֌W�̏����L�q.
*/
#pragma once

/**
* ���j�`�b�vID.
*/
enum class ChipID : int
{
	None = -1,	//!< �ݒ�Ȃ�.
	Fire,		//!< �΂̃`�b�v.
	Iron,		//!< �S�̃`�b�v.
	Oil,		//!< �Ζ��̃`�b�v.
	ChipNum,	//!< �`�b�v�̐�.
};

/**
* �`�b�v�̖��O.
*/
static const string ChipName[(int)ChipID::ChipNum] =
{
	"�΂̃`�b�v",
	"�S�̃`�b�v",
	"�Ζ��̃`�b�v",
};

/**
* �`�b�v�̃e�N�X�`����
*/
static const string ChipFileName[(int)ChipID::ChipNum] = 
{
	"FirePage.png",
	"IronPage.png",
	"OilPage.png",
};

/**
* �ꏊ�R�[�h.
*/
enum class LocationCodeE : int
{
	Begin = 0,		//!< �͂��܂�̏W��.
	Hunting,		//!< ��̑�.
	Prosperity,		//!< �ɉh�̒�.
	LocationNum,	//!< �ꏊ�̐�.
};

/** 
* �ꏊ��. 
*/
static const string LocationNameList[(int)LocationCodeE::LocationNum] =
{
	"�͂��܂�̏W��",
	"��̑�",
	"�ɉh�̒�",
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

	bool SetChip(ChipID chipID, int slot)
	{
		if (_ChipSlot[slot] == ChipID::None)
		{
			_ChipSlot[slot] = chipID;
		}
		else
		{
			//���ɂ��炷.
			//���炩�Ƀo�O.
			_ChipSlot[slot + 1] = _ChipSlot[slot];
			_ChipSlot[slot] = chipID;
		}
		return 0;
	}

public:

	/** �ꏊID. */
	LocationCodeE _LocationID;
	/** �o�^����Ă�����j�`�b�vID���X�g. */
	ChipID _ChipSlot[(int)ChipID::ChipNum];

};

namespace
{

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_LocationID),	sizeof(int) },
		{ "Slot[0]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[0]),	sizeof(int) },
		{ "Slot[1]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[1]),	sizeof(int) },
		{ "Slot[2]",	Support::DataTypeE::INT, offsetof(struct LocationHistoryInfo,_ChipSlot[2]),	sizeof(int) }
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
	const Support::DATARECORD VillageGroupData[4] =
	{
		{ "GroupID",Support::DataTypeE::INT, offsetof(struct VillageGroup,GroupID),sizeof(int) },
		{ "Slot[0]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[0]),sizeof(ChipID) },
		{ "Slot[1]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[1]),sizeof(ChipID) },
		{ "Slot[2]",Support::DataTypeE::INT, offsetof(struct VillageGroup,Slot[2]),sizeof(ChipID) },
	};

}