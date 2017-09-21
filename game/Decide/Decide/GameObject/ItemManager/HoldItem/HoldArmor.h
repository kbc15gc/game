#pragma once
#include "HoldEquipment.h"

//�����h��̃N���X�B
class HoldArmor :public HoldEquipment
{
public:
	HoldArmor(Item::BaseInfo* info);
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

	//�h��̃����N���Z�o�B
	inline float ParamRaitoMass()override {
		float offset = _DefRnd + _MDefRnd;
		float sum = _Def + _MagicDef;
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
		_DefRnd = baseParam * raito;
		_Def = baseParam + _DefRnd;
	}

	//�����N���l���������@�h��͂��v�Z�B
	inline void RndMDef() {
		// ���@�h��͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
		int rnd = GetRand_S50to100();	// -50����100�̒l�������_���Ŏ擾�B
		float raito = static_cast<float>(rnd) * 0.01f;

		//�ŏI�I�Ȗ��@�h��́B
		_MDefRnd = baseParam * raito;
		_MagicDef = baseParam + _MDefRnd;
	}
private:
	int _DefRnd;	//�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MDefRnd;	//���@�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _Def;		//�����N���l�����������h��́B
	int _MagicDef;	//�����N���l���������@�h��́B
};