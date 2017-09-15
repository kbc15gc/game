#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�����h��̃N���X�B
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();

	//�h��̓Ǝ��p�����[�^�[��ݒ�B
	//�����F�h��̗͂��������A���@�h��̗͂��������B
	inline void SetParam(float Def, float MDef) {
		_Def += _Def*Def;
		_MagicDef += _MagicDef*MDef;
	}

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
private:
	int _DefRnd;	//�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MDefRnd;	//���@�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _Def;		//�����N���l�����������h��́B
	int _MagicDef;	//�����N���l���������@�h��́B
};