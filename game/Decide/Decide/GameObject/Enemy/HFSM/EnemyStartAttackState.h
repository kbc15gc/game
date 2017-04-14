#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// エネミー攻撃開始ステート。
class EnemyStartAttackState : public EnemyState {
public:
	EnemyStartAttackState(EnemyCharacter* Object);
	~EnemyStartAttackState();
	void Exit(EnemyCharacter::State next)override;
private:
	void _EntrySubClass()override;

	void _Start()override;

	// 度の攻撃をするか判定する関数。
	// 攻撃パターンを増やした際にはここに分岐を記載。
	// 判定用のAI処理もここで呼び出すか記載する。
	void _AttackSelect();

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);
};