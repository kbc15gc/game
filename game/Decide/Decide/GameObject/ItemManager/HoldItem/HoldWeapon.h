#pragma once
#include "HoldEquipment.h"

//�������Ă��镐��̃N���X�B
class HoldWeapon :public HoldEquipment
{
private:
	friend class HoldItemFactory;
	HoldWeapon(char* name);
public:
	~HoldWeapon();


	//����̃p�����[�^�[�������_���ŎZ�o�B
	void CreateRandParam()override;

	//����̃p�����[�^�[����l�Őݒ�B
	void CreateOriginParam() override;

	//�U���̗͂����������擾�B
	inline int GetAtkRnd() const {
		return _AtkRnd;
	}

	//���@�U���̗͂����������擾�B
	inline int GetMtkRnd() const{
		return _MAtkRnd;
	}

	//�N���e�B�J�����̗����������擾�B
	inline int GetDexRnd() const{
		return _DexRnd;
	}

	//�����N���l�����������U����(�N���e�B�J�����͏���)���擾�B
	inline int GetAtk() const{
		return _Atk;
	}

	//�����N���l���������@�U���̓N���e�B�J�����͏���)���擾�B
	inline int GetMagicAtk()const {
		return _MagicAtk;
	}

	//�����N���l�������N���e�B�J�������擾�B
	inline int GetDex()const {
		return _Dex;
	}

	// �N���e�B�J���З͎擾�B
	inline int GetCrt()const {
		return _Crt;
	}

	////����̊�l�ƍ����l�̊������Z�o�B
	//inline float SumRaitoMass()override {
	//	float offset = static_cast<float>(_AtkRnd + _MAtkRnd + _DexRnd);
	//	float sum = static_cast<float>(_Atk + _MagicAtk + _Dex);
	//	float par = offset / (sum - offset);
	//	return par;
	//}

	//�����N���l�����������_�������U���͂̌v�Z�B
	// �����F	�������B
	inline void RndAtkMass(float offset) {
		//�����U���͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Atk;

		//�ŏI�I�ȕ����U���͂��Z�o�B
		_AtkRnd = static_cast<int>(baseParam * offset);
		_Atk = baseParam + _AtkRnd;
	}

	//�����N���l�����������_�����@�U���͂̌v�Z�B
	// �����F	�������B
	inline void RndMAtkMass(float offset) {
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->MagicAtk;

		//�ŏI�I�Ȗ��@�U���͂��Z�o�B
		_MAtkRnd = static_cast<int>(baseParam * offset);
		_MagicAtk = baseParam + _MAtkRnd;
	}

	//�����N���l�����������_���N���e�B�J�����̌v�Z�B
	// �����F	�������B
	inline void RndDexMass(float offset) {
		//�ŏI�I�ȃN���e�B�J���̃����_�������Z�o�B
		int baseParam = static_cast<Item::WeaponInfo*>(_Info)->Dex;

		//�ŏI�I�ȃN���e�B�J�������Z�o�B
		_DexRnd = static_cast<int>(baseParam * offset);
		_Dex = baseParam + _DexRnd;
	}



private:

	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _AtkRnd;		//�U���̗͂�������(���̒l�Ń����N�t��)�B
	int _MAtkRnd;		//���@�U���̗͂�������(���̒l�Ń����N�t��)�B
	int _DexRnd;		//�N���e�B�J�����̗�������(���̒l�Ń����N�t��)�B
	int	_Atk;			//�����N���l�����������U���́B
	int	_MagicAtk;		//�����N���l���������@�U���́B
	int _Dex;			//�����N���l�������N���e�B�J�����B
	int _Crt;			//�N���e�B�J���З́B
};