#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "EnemyAttackState.h"

EnemyStartAttackState::EnemyStartAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
}


EnemyStartAttackState::~EnemyStartAttackState()
{
}

void EnemyStartAttackState::_EntrySubClass() {

}

void EnemyStartAttackState::_Start() {
	// エネミーにどの攻撃を行うかを判断させる。
	// ※エネミーの攻撃パターンを選別するステートを作ってしまうと、
	//   エネミーの種類に応じてステートが爆発的に増えてしまうため、攻撃パターンの選別は各自エネミーに行わせる。
	_EnemyObject->AttackSelect();

	// 攻撃可能範囲まで追跡させる。
	_ChangeLocalState(EnemyCharacter::State::Chace);
}

void EnemyStartAttackState::_UpdateSubClass() {

}

void EnemyStartAttackState::Exit(EnemyCharacter::State next) {
	if (_NowLocalState) {
		_NowLocalState->Exit(next);
	}
}

void EnemyStartAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Chace) {
		// 攻撃範囲内。

		// 攻撃開始。
		_ChangeLocalState(EnemyCharacter::State::Attack);
	}
	else if (EndLocalStateType == EnemyCharacter::State::Attack) {
		// 攻撃終了。

		_EndState();
	}
}
