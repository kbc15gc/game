#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int mp, int atk, int def, int dex, int agi)
{
	//�p�����[�^�[�ݒ�B
	_HP = hp;		//�q�b�g�|�C���g
	_MP = mp;		//�}�W�b�N�|�C���g
	_ATK = atk;		//�U����
	_DEF = def;		//�h���
	_DEX = dex;		//������
	_AGI = agi;		//����
}
