#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�����i�̊��N���X�B
class HoldEquipment :public HoldItemBase
{
public:
	//�R���X�g���N�^�B
	HoldEquipment(Item::BaseInfo* info);

	//�f�X�g���N�^�B
	~HoldEquipment();

	// �R�����g�̐����͊�l����̉��Z���̕��B
	enum Rank
	{
		SS = 0, //91 ~ 100	�_�����B
		S,		//71 ~ 90	�󕨋��B
		A,		//51 ~ 70	�����B
		B,		//31 ~ 50	�ǁB
		C,		//-20 ~ 30	���ρB
		D,		//-40 ~ -21	�e���B
		E,		//-50 ~ -41	���e���B
	};

	//����܂��͖h��̃����N������B
	inline void RankSelect(float raito) {
		if (raito >= -0.5f && raito <= -0.41f) {
			_Rank = Rank::E;
		}
		else if (raito >= -0.4f && raito <= -0.21f) {
			_Rank = Rank::D;
		}
		else if (raito >= -0.2f && raito <= 0.3f) {
			_Rank = Rank::C;
		}
		else if (raito >= 0.31f && raito <= 0.5f) {
			_Rank = Rank::B;
		}
		else if (raito >= 0.51f && raito <= 0.7f) {
			_Rank = Rank::A;
		}
		else if (raito >= 0.71f && raito <= 0.9f) {
			_Rank = Rank::S;
		}
		else if (raito >= 0.9f && raito <= 1.0f) {
			_Rank = Rank::SS;
		}

	}

	//����������͖h��̃����N���Z�o�B
	virtual inline float ParamRaitoMass() {
		return 0.0f;
	}

	//����A�h��̃p�����[�^�[�������_���ŎZ�o�B
	virtual void CreateRandParam() {
		
	}

	//����A�h��̃p�����[�^�[����l�Őݒ�B
	virtual void CreateOriginParam() {

	}

	//��������Ȃ炱�̊֐����ĂԁB
	inline void SetIsEquipTrue() {
		_IsEquip = true;
	}

	//�������Ȃ��Ȃ炱�̊֐����ĂԁB
	inline void SetIsEquipFalse() {
		_IsEquip = false;
	}

	//�����t���O���Z�b�g(ture�Ȃ瑕�����Ă���Afalse�Ȃ瑕�����Ă��Ȃ�)�B
	inline bool GetIsEquip() {
		return _IsEquip;
	}

protected:
	Rank _Rank;					//�����i�̃����N�B
	bool _IsEquip = false;		//��������Ă��邩�̃t���O(ture�Ȃ瑕������Ă���)�B
};

