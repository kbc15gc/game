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

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void LateUpdate()override;

	void _ExitSubClass(EnemyCharacter::State next)override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

	inline bool IsPossibleChangeState(EnemyCharacter::State next)override {
		if (next == EnemyCharacter::State::Chace || next == EnemyCharacter::State::Discovery) {
			return true;
		}
		return false;
	}

	inline bool IsPossibleDamage()override {
		return false;
	}
private:
	NPC* _speakObject = nullptr;
};