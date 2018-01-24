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
		SS = 0, // �_�����B
		S,		// �󕨋��B
		A,		// �����B
		B,		// �ǁB
		C,		// ���ρB
		None,	// ��������.
		Max,	// ��.
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

	//// �␳�l���瑕���i�̃����N�����肷��B
	//// �����F	�␳�l�B
	//inline void RankSelect(float ratio) {
	//	if (ratio >= 0.26f && ratio <= 0.5f) {
	//		// 26% �` 50%�B
	//		_Rank = Rank::SS;
	//	}
	//	else if (ratio >= 0.16f && ratio <= 0.25f) {
	//		// 16% �` 25%�B
	//		_Rank = Rank::S;
	//	}
	//	else if (ratio >= 0.11f && ratio <= 0.15f) {
	//		// 11% �` 15%�B
	//		_Rank = Rank::A;
	//	}
	//	else if (ratio >= 0.06f && ratio <= 0.1f) {
	//		// 6% �` 10%�B
	//		_Rank = Rank::B;
	//	}
	//	else if (ratio >= -0.19f && ratio <= 0.05f) {
	//		// -19% �` 5%�B
	//		_Rank = Rank::C;
	//	}
	//	else if (ratio >= -0.34f && ratio <= -0.2f) {
	//		// -34% �` -20%�B
	//		_Rank = Rank::D;
	//	}
	//	else if (ratio >= -0.5f && ratio <= -0.35f) {
	//		// -50% �` -35%�B
	//		_Rank = Rank::E;
	//	}
	//	else {
	//		_Rank = Rank::None;
	//	}

	//	//�␳�l�i�[�B
	//	_Revision = ratio;
	//}

	//����܂��͖h��̃����N�����肵�A�␳�l���Z�o����B
	inline void RankSelect() {
		int rnd = rand() % 10000;	// 0.01%�`100.00%�̊m�����v�Z�B

		if (rnd >= 0 && rnd <= 999) {
			// 10.00%�B
			_Rank = Rank::SS;
			_Revision = ParamRaitoMass(26, 50);
		}
		else if (rnd >= 1000 && rnd <= 2999) {
			// 20.00%�B
			_Rank = Rank::S;
			_Revision = ParamRaitoMass(16,25);
		}
		else if (rnd >= 3000 && rnd <= 7999) {
			// 50.00%�B
			_Rank = Rank::A;
			_Revision = ParamRaitoMass(11, 15);
		}
		else if (rnd >= 8000 && rnd <= 9999) {
			// 20.00%�B
			_Rank = Rank::B;
			_Revision = ParamRaitoMass(6, 10);
		}
	}

	//�����P��������Q�̊Ԃő����̍����l�̊������Z�o�B
	// �����F	����(%)�B
	//			���(%)�B
	inline float ParamRaitoMass(int min,int max) {
		int rnd;
		rnd = rand() % ((max - min) + 1) + min;
		return static_cast<float>(rnd) * 0.01f;
	}

	virtual inline float SumRaitoMass() { return 0.0f; };

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
	/*int GetValue()override
	{
		return  _Info->Value + _Info->Value * GetRevision();
	}*/
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
	"None",
};