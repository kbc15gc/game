#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int mp, int atk, int def, int dex, int agi)
{
	//�p�����[�^�[�ݒ�B
	_Param[Param::HP] = hp;		//�q�b�g�|�C���g
	_Param[Param::MP] = mp;		//�}�W�b�N�|�C���g
	_Param[Param::ATK] = atk;		//�U����
	_Param[Param::DEF] = def;		//�h���
	_Param[Param::DEX] = dex;		//������
	_Param[Param::AGI] = agi;		//����
}
