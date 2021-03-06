#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "GhostPairAttackState.h"

GhostPairAttackState::GhostPairAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
	_playAnimation = EnemyCharacter::AnimationType::Idle;
	_loopNum = -1;
}


GhostPairAttackState::~GhostPairAttackState()
{
}

void GhostPairAttackState::_EntrySubClass() {
	_EnemyObject->ConfigDamageReaction(false);
	_isEndWarp = false;
	_isStartAttackAlready = false;
	_counter = 0.0f;
	_isStartAttack = false;
}

void GhostPairAttackState::_StartSubClass() {
	// エネミーにどの攻撃を行うかを判断させる。
	// ※エネミーの攻撃パターンを選別するステートを作ってしまうと、
	//   エネミーの種類に応じてステートが爆発的に増えてしまうため、攻撃パターンの選別は各自エネミーに行わせる。
	_EnemyObject->AttackSelect();
}

void GhostPairAttackState::_UpdateSubClass() {
	if (!_isEndWarp) {
		// 透過処理でゴーストを消す。
		// ※他の側近が別の行動をしている際のにタイミングを合わせるために行う。

		float alpha = _EnemyObject->GetAlpha();

		alpha -= 1.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// 完全に透明になった。

			alpha = 0.0f;
			_isEndWarp = true;
		}
		_EnemyObject->SetAlpha(alpha);
	}
	else if ((!_isStartAttackAlready) && _isStartAttack) {
		// 側近がすべて消えたので攻撃開始。

		if (_counter >= _startAttackInterval) {
			// 開始時間を他の側近とずらす。
			_ChangeLocalState(EnemyCharacter::State::Attack);
			_isStartAttackAlready = true;
		}
		else {
			_counter += Time::DeltaTime();
		}
	}
}

void GhostPairAttackState::_ExitSubClass(EnemyCharacter::State next) {
}

void GhostPairAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Attack) {
		// 攻撃終了。

		_EndState();
	}
}
