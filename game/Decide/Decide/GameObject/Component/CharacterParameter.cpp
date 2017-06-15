#include "stdafx.h"
#include "CharacterParameter.h"

void CharacterParameter::ParamInit(int hp, int maxhp, int mp, int maxmp, int atk, int def, int dex, int agi)
{
	//�p�����[�^�[�ݒ�B
	_Param[Param::HP]	= hp;		//�q�b�g�|�C���g�B
	_Param[Param::MAXHP]= maxhp;	//�q�b�g�|�C���g�B
	_Param[Param::MP]	= mp;		//�}�W�b�N�|�C���g�B
	_Param[Param::MAXMP]= maxmp;	//�q�b�g�|�C���g�B
	_Param[Param::ATK]	= atk;		//�U���́B
	_Param[Param::DEF]	= def;		//�h��́B
	_Param[Param::DEX]	= dex;		//�����́B
	_Param[Param::AGI]	= agi;		//���́B
}

void CharacterParameter::DamageMass(int atk, int def)
{
	int Damage = 0;

	//�_���[�W=�U����-(0.5(�␳�l�I�ȉ����ɂ��̂����u��������)*�h���)�B
	Damage = atk - (0.5f*def);

	//�̗͂��v�Z�����_���[�W�����Z�B
	SubParam(Param::HP, Damage);

	//�̗͂�0�ȉ��ɂȂ����̂Ŏ��񂾁B
	if (_Param[Param::HP] <= 0)
	{
		_DeathFlag = true;
	}

}