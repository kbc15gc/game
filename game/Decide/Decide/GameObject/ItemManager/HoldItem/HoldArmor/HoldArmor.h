#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�����h��̃N���X�B
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();

	//�h��̓Ǝ��p�����[�^�[��ݒ�B
	//�����F�h��́A���@�h��́B
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
private:
	int _DefRnd;	//�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _MDefRnd;	//���@�h��̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	int _Def;		//�����N���l�����������h��́B
	int _MagicDef;	//�����N���l���������@�h��́B
};