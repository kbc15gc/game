#pragma once
#include "HoldEquipment.h"

//�������Ă��镐��̃N���X�B
class HoldWeapon :public HoldEquipment
{
public:
	HoldWeapon(char* name);
	~HoldWeapon();


	//����̃p�����[�^�[�������_���ŎZ�o�B
	void CreateRandParam()override;

	//����̃p�����[�^�[����l�Őݒ�B
	void CreateOriginParam() override;

	//�U���̗͂����������擾�B
	inline int GetAtkRnd() {
		return _AtkRnd;
	}

	//���@�U���̗͂����������擾�B
	inline int GetMtkRnd() {
		return _MAtkRnd;
	}

	//�N���e�B�J�����̗����������擾�B
	inline int GetCrtRnd() {
		return _DexRnd;
	}

	//�����N���l�����������U����(�N���e�B�J�����͏���)���擾�B
	inline int GetAtk() {
		return _Atk;
	}

	//�����N���l���������@�U���̓N���e�B�J�����͏���)���擾�B
	inline int GetMagicAtk() {
		return _MagicAtk;
	}

	//�����N���l�������N���e�B�J�������擾�B
	inline int GetDex() {
		return _Dex;
	}

	//����̊�l�ƍ����l�̊������Z�o�B
	inline float ParamRaitoMass()override {
		float offset = _AtkRnd + _MAtkRnd + _DexRnd;
		float sum = _Atk + _MagicAtk + _Dex;
		float par = offset / sum;
		return par;
	}

	//�����N���l�����������_�������U���͂̌v�Z�B
	inline void RndAtkMass() {
		//�����U���͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Atk;
		int rnd = GetRand_S50to100();// -50����100�̒l�������_���Ŏ擾�B
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�ȕ����U���͂��Z�o�B
		_AtkRnd = baseParam * raito;
		_Atk = baseParam + _AtkRnd;
	}

	//�����N���l�����������_�����@�U���͂̌v�Z�B
	inline void RndMAtkMass() {
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;
		int rnd = GetRand_S50to100();	// -50����100�̒l�������_���Ŏ擾�B
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�Ȗ��@�U���͂��Z�o�B
		_MAtkRnd = baseParam * raito;
		_MagicAtk = baseParam + _MAtkRnd;
	}

	//�����N���l�����������_���N���e�B�J�����̌v�Z�B
	inline void RndCrtMass() {
		//�ŏI�I�ȃN���e�B�J���̃����_�������Z�o�B
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Dex;
		int rnd = GetRand_S50to100();
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�ȃN���e�B�J�������Z�o�B
		_DexRnd = baseParam * raito;
		_Dex = baseParam + _DexRnd;
	}



private:

	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _AtkRnd;		//�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MAtkRnd;		//���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _DexRnd;		//�N���e�B�J�����̗�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int	_Atk;			//�����N���l�����������U���́B
	int	_MagicAtk;		//�����N���l���������@�U���́B
	int _Dex;			//�����N���l�������N���e�B�J�����B
};