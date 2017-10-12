#pragma once
#include "EnemyState.h"
#include "GameObject\Village\NPC.h"

// エネミー会話ステート。
class EnemySpeakState :public EnemyState {
public:
	EnemySpeakState(EnemyCharacter* Object);
	~EnemySpeakState();
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		if (next == EnemyCharacter::State::Chace || next == EnemyCharacter::State::Discovery) {
			return true;
		}
		return false;
	}
private:
	NPC* _speakObject = nullptr;
};