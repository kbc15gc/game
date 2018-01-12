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

	////�h��̊�l�ƍ����l�̊������Z�o�B
	//inline float SumRaitoMass()override {
	//	float offset = static_cast<float>(_DefRnd + _MDefRnd);
	//	float sum = static_cast<float>(static_cast<Item::ArmorInfo*>(_Info)->Def + static_cast<Item::ArmorInfo*>(_Info)->MagicDef);
	//	float par = offset / sum;
	//	return par;
	//}

	//�����N���l�����������h��͂��v�Z�B
	// �����F	�������B
	inline void RndDefMass(float offset) {
		// �����h��͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->Def;

		//�ŏI�I�ȕ����h��͂��Z�o�B
		_DefRnd = static_cast<int>(baseParam * offset);
		_Def = baseParam + _DefRnd;
	}

	//�����N���l���������@�h��͂��v�Z�B
	// �����F	�������B
	inline void RndMDef(float offset) {
		// ���@�h��͂̃����_�������Z�o�B
		int baseParam = static_cast<Item::ArmorInfo*>(_Info)->MagicDef;
		//�ŏI�I�Ȗ��@�h��́B
		_MDefRnd = static_cast<int>(baseParam * offset);
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