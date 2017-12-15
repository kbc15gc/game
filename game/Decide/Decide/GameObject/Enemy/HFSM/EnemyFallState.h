#pragma once
#include "EnemyState.h"


// �G�l�~�[�����X�e�[�g�B
class EnemyFallState :public EnemyState{
public:
	EnemyFallState(EnemyCharacter* Object);
	~EnemyFallState();
private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;
	
	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	// ���̃X�e�[�g����ڍs�ł���X�e�[�g��I�ʂ���֐��B
	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		if (next == EnemyCharacter::State::Damage) {
			return false;
		}
		return true;
	}

	// ���̃X�e�[�g���Ƀ_���[�W��^�����邩�B
	inline virtual bool IsPossibleDamage() {
		return false;
	}

};