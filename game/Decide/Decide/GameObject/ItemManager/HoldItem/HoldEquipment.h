#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"



//�����i�̊��N���X�B
class HoldEquipment :public HoldItemBase
{
public:
	// �R�����g�̐����͊�l����̉��Z���̕��B
	//@todo for debug
	//��Œ������ĂˁB
	enum Rank
	{
		SS = 0, //91 ~ 100	�_�����B
		S,		//71 ~ 90	�󕨋��B
		A,		//51 ~ 70	�����B
		B,		//31 ~ 50	�ǁB
		C,		//-20 ~ 30	���ρB
		D,		//-40 ~ -21	�e���B
		E,		//-50 ~ -41	���e���B
		None,	//��������.
		Max,	//��.
	};

protected:
	friend class HoldItemFactory;
	HoldEquipment(char* name);
public:
	~HoldEquipment();


	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	virtual void ConfigLoadData(Hold::HoldInfo* info);

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
		else if (raito >= 0.91f && raito <= 1.0f) {
			_Rank = Rank::SS;
		}
		//�␳�l�i�[�B
		_Revision = raito;
	}

	//�����̊�l�ƍ����l�̊������Z�o�B
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

	//�����t���O���擾(ture�Ȃ瑕�����Ă���Afalse�Ȃ瑕�����Ă��Ȃ�)�B
	inline bool GetIsEquip() {
		return _IsEquip;
	}

	Rank GetRank()
	{
		return _Rank;
	}

	//�␳�l���擾�B
	float GetRevision()
	{
		return _Revision;
	}

	//�l�i�擾�B
	int GetValue()override
	{
		return  _Info->Value + _Info->Value * GetRevision();
	}
private:
	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// �p����ɂ���ď�����ύX�B
	virtual void _ConfigLoadDataSubClass(Hold::HoldEquipInfo* info) = 0;

protected:
	Rank _Rank;	//�����i�̃����N�B
	
	float _Revision = 0.0f;		//�␳�l�B
	bool _IsEquip = false;		//��������Ă��邩�̃t���O(ture�Ȃ瑕������Ă���)�B

	bool _isLoad = false;	// CSV����ǂݍ��񂾃f�[�^���B
};

static char* RankText[HoldEquipment::Rank::Max] =
{
	"SS",
	"S",
	"A",
	"B",
	"C",
	"D",
	"E",
	"None",
};