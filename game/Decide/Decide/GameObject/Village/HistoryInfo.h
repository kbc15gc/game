#pragma once

//���j�`�b�v�̎��
enum class ChipID : int
{
	NONE,	//�O��
	FIRE,	//�΂̃`�b�v
	IRON,	//�S�̃`�b�v
	OIL,	//�Ζ��̃`�b�v�H
	NUM,
};

	//�e�嗤�̗��j���_���̏�
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
	//���j�`�b�v���Z�b�g����֐��B
	//�Z�b�g����ꍇ�͕K�����̊֐����g���悤�ɂ���B
	//[in] unsigned int ���ԖڂɃZ�b�g���邩�̓Y����
	//[in] ChipID	���̃`�b�v���Z�b�g���邩�H
	//[out] bool �Z�b�g�ł������ǂ����H
	bool SetChip(const unsigned int& idx, const ChipID& chip)
	{
		//�͈͓��`�F�b�N
		if (idx < HISTORY_CHIP_NUM)
		{
			//�����`�b�v��ݒ肷��ꍇ�B
			if (chip != ChipID::NONE)
			{
				//�����`�b�v�𕡐����Ȃ��悤�Ƀ`�F�b�N�B
				for each (ChipID Chip in Slot)
				{
					//���ɂ����Ă����̂ŏI���B
					if (Chip == chip)
						return false;
				}
			}
			//�����ݒ�B
			Slot[idx] = chip;
			return true;
		}
		return false;
	}
public:
	//�嗤��ID
	int ContinentID;
	//���j�`�b�v��ID
	ChipID Slot[HISTORY_CHIP_NUM];
};

namespace
{
	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT , offsetof(struct HistoryInfo,ContinentID),sizeof(int) },
		{ "Slot[0]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[0]),sizeof(int) },
		{ "Slot[1]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[1]),sizeof(int) },
		{ "Slot[2]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Slot[2]),sizeof(int) }
	};

	//�I�u�W�F�N�g�̏��
	struct VillageGroup
	{
		//�O���[�vID
		int GroupID;
		//�e�X���b�g�̐ݒu��
		ChipID Slot[3];
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