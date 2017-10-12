#pragma once
#include "HoldEquipment.h"

//�����h��̃N���X�B
class HoldArmor :public HoldEquipment
{
private:
	friend class HoldItemFactory;
	HoldArmor(char* name);
public:
	~HoldArmor();

	//�h��̃p�����[�^�[�������_���ŎZ�o�B
	void CreateRandParam()override;

	//�h��̃p�����[�^�[����l�Őݒ�B
	void CreateOriginParam() override;

	//�h��̗͂����������擾�B
	inline int GetDefRnd() {
		return _DefRnd;
	}

	//���@�h��̗͂����������擾�B
	inline int GetMDefRnd() {
		return _MDefRnd;
	}

	//�����N���l�����������h��͂��擾�B
	inline int GetDef() {
		return _Def;
	}

	//�����N���l���������@�h��͂��擾�B
	inline int GetMagicDef() {
		return _MagicDef;
	}

	//�h��̊�l�ƍ����l�̊������Z�o�B
	inline float ParamRaitoMass()override {
		float offset = static_cast<float>(_DefRnd + _MDefRnd);
		float sum = static_cast<float>(_Def + _MagicDef);
		float par = offset / sum;
		return par;
	}

	//�����N���l�����������h��͂��v�Z�B
	inline void RndDefMass() {
		// �����h��͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->Def;
		int rnd = GetRand_S50to100();// -50����100�̒l�������_���Ŏ擾�B
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�ȕ����h��͂��Z�o�B
		_DefRnd = static_cast<int>(baseParam * raito);
		_Def = baseParam + _DefRnd;
	}

	//�����N���l���������@�h��͂��v�Z�B
	inline void RndMDef() {
		// ���@�h��͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
		int rnd = GetRand_S50to100();	// -50����100�̒l�������_���Ŏ擾�B
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�Ȗ��@�h��́B
		_MDefRnd = static_cast<int>(baseParam * raito);
		_MagicDef = baseParam + _MDefRnd;
	}

private:
	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info)override;

private:
	int _DefRnd = 0;	//�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MDefRnd = 0;	//���@�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _Def;		//�����N���l�����������h��́B
	int _MagicDef;	//�����N���l���������@�h��́B
};