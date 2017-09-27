#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include "fbEngine\_Object\_GameObject\CollisionObject.h"
#include "GameObject\Component\ParticleEffect.h"
class GameObject;

//����A�C�e���̃N���X�B
class ConsumptionItem :public HoldItemBase
{
public:
	enum class EffectType{Heel = 0,Buff,Debuff};
	// �A�C�e�����y�ڂ��ΏہB
	enum class Range{
		I = 0,	// 1�́B
		III,	// 3�́B
		V,		// 5�́B
		All,	// �͈͓����ׂāB
		Max
	};
public:
	ConsumptionItem(char* name);
	~ConsumptionItem();

	void Start()override;

	//�A�C�e�����g���B
	void UseItem();

	//���������X�V(���炷�ꍇ�͈����Ƀ}�C�i�X��ݒ�)�B
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//���������擾�B
	inline int GetHoldNum() {
		return  _HoldNum;
	}

private:

	void SetParticleBuffParam();

	GameObject* _user = nullptr;	// �g�p�ҁB

	float _range = 10.0f;	// ���ʂ��y�ڂ��͈�(���͈̔͊O�̂��̂ɂ̓A�C�e���͉e�����y�ڂ��Ȃ�)�B
	int _HoldNum = 0;	// �������B

	CollisionObject* _gost = nullptr;	// �Ώۂ̒T���ɕK�v�B

	ParticleEffect* _Effect = nullptr;
};