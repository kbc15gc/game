#pragma once
#include "EnemyState.h"

class BossGhost;
class Player;

// ���X�{�X�ʍ�(���ߏ����A�v���C���[�Ƀf�o�t�A�������G���)�X�e�[�g�B
class LastBossThroneState : public EnemyState {
public:
	LastBossThroneState(EnemyCharacter* Object);
	~LastBossThroneState();

	// ���߂ɋ����U�����߁B
	bool EntourageCommand();
	// ���߂Ƀo�t�B
	void EncourageBuff();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// ���̃X�e�[�g����ڍs�ł���X�e�[�g��I�ʂ���֐��B
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		if (next == EnemyCharacter::State::Damage || next == EnemyCharacter::State::Death || next == EnemyCharacter::State::Fall) {
			return false;
		}
		return true;
	}

	// ���̃X�e�[�g���Ƀ_���[�W��^�����邩�B
	inline virtual bool IsPossibleDamage() {
		return false;
	}


private:
	bool _isOutsideRange = false;	// �͈͊O�ɏo�����B
	float _timeCounter;
	float _interval = 10.0f;

	bool _isDeathEntourage = false;
	static const int _entourageNum;	// ���߂̐��B
	vector<BossGhost*> _entourageEnemys;	// ���߁B
	Player* _player = nullptr;
};