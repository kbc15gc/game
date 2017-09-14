#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�������Ă��镐��̃N���X�B
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(Item::BaseInfo* info);
	~HoldWeapon();

	//����̓Ǝ��p�����[�^�[��ݒ�B
	//�����F�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�A���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	inline void SetParam(float AtkRnd, float MAtkRnd) {
		_AtkRnd = AtkRnd;
		_MAtkRnd = MAtkRnd;
		_Atk += _Atk * AtkRnd;
		_MagicAtk += _MagicAtk*MAtkRnd;
	}

	//�U���̗͂����������擾�B
	inline int GetAtkRnd() {
		return _AtkRnd;
	}

	//���@�U���̗͂����������擾�B
	inline int GetMtkRnd() {
		return _MAtkRnd;
	}

	//�����N���l�����������U����(�N���e�B�J�����͏���)���擾�B
	inline int GetAtk() {
		return _Atk;
	}

	//�����N���l���������@�U���̓N���e�B�J�����͏���)���擾�B
	inline int GetMagicAtk() {
		return _MagicAtk;
	}

private:
	int _AtkRnd;		//�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MAtkRnd;		//���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int	_Atk;			//�����N���l�����������U���́B
	int	_MagicAtk;		//�����N���l���������@�U���́B
};