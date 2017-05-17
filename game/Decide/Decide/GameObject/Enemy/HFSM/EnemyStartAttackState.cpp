#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"

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
	Vector3 EnemyToPlayer(_Player->transform->GetPosition() - _EnemyObject->transform->GetPosition());

	if (false) {
		// プレイヤーが死亡した。
	}
	else if (EnemyToPlayer.Length() >= _EnemyObject->GetAttackRange()) {
		// プレイヤーが攻撃範囲外に離脱した。

		// 再び追跡させる。
		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Discovery);
	}
	else {
		// プレイヤーを攻撃可能。

		// エネミーにどの攻撃を行うかを判断させる。
		// ※エネミーの攻撃パターンを選別するステートを作ってしまうと、
		//   エネミーの種類に応じてステートが爆発的に増えてしまうため、攻撃パターンの選別は各自エネミーに行わせる。
		_ChangeLocalState(_EnemyObject->AttackSelect());
	}
}

void EnemyStartAttackState::_UpdateSubClass() {

}

void EnemyStartAttackState::Exit(EnemyCharacter::State next) {
}

void EnemyStartAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// 攻撃終了。

	_EndState();
}
