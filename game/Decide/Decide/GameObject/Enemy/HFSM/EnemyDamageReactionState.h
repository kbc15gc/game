#pragma once

#include "EnemyState.h"


// �G�l�~�[�_���[�W�����X�e�[�g�B
class EnemyDamageReactionState : public EnemyState {
public:
	EnemyDamageReactionState(EnemyCharacter* Object) : EnemyState(Object) {};
	~EnemyDamageReactionState() {};
private:
	void _EntrySubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next) {
		if (next == EnemyCharacter::State::Fall || next == EnemyCharacter::State::Death) {
			return true;	// �����Ǝ��S�̂݋��B
		}
		else {
			return false;
		}
	}

};