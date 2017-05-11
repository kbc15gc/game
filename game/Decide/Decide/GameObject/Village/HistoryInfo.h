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
	//���j�`�b�v���Z�b�g����֐��B
	//�Z�b�g����ꍇ�͕K�����̊֐����g���悤�ɂ���B
	//[in] unsigned int ���ԖڂɃZ�b�g���邩�̓Y����
	//[in] ChipID	���̃`�b�v���Z�b�g���邩�H
	//[out] bool �Z�b�g�ł������ǂ����H
	bool SetChip(const unsigned int& idx,const ChipID& chip)
	{
		//�͈͓��`�F�b�N
		if (idx < HISTORY_CHIP_NUM)
		{
			//�����`�b�v��ݒ肷��ꍇ�B
			if (chip != ChipID::NONE)
			{
				//�����`�b�v�𕡐����Ȃ��悤�Ƀ`�F�b�N�B
				for each (ChipID Chip in Chips)
				{
					//���ɂ����Ă����̂ŏI���B
					if (Chip == chip)
						return false;
				}
			}
			//�����ݒ�B
			Chips[idx] = chip;
			return true;
		}
		return false;
	}
public:
	//�嗤��ID
	int ContinentID;
	//���j�`�b�v��ID
	ChipID Chips[HISTORY_CHIP_NUM];
	//���j�O���[�v
	int GroupID;
};

namespace
{
	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD HistoryInfoData[4] =
	{
		{ "ContinentID",Support::DataTypeE::INT , offsetof(struct HistoryInfo,ContinentID),sizeof(int) },
		{ "Chips[0]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[0]),sizeof(int) },
		{ "Chips[1]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[1]),sizeof(int) },
		{ "Chips[2]",Support::DataTypeE::INT, offsetof(struct HistoryInfo,Chips[2]),sizeof(int) }
	};
}