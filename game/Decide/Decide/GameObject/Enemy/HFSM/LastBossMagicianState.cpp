#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossMagicianState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\LastBoss.h"

LastBossMagicianState::LastBossMagicianState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossMagicianState::~LastBossMagicianState()
{
}

void LastBossMagicianState::_EntrySubClass() {
	// 無敵解除。

	_ChangeLocalState(EnemyCharacter::State::StartAttack);
	_timeCounter = 0.0f;
}

void LastBossMagicianState::_StartSubClass() {
	// のけぞり設定。
	//_EnemyObject->ConfigDamageReaction(true,30);
	_EnemyObject->ConfigDamageReaction(true, 1);
}

void LastBossMagicianState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。



	if (_timeCounter >= _interval) {
		// 一定時間で歴史改ざんに移行しステート終了。

		_EndState();
	}
	else {
		_timeCounter += Time::DeltaTime();
	}
}

void LastBossMagicianState::_ExitSubClass(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Damage) {
		static_cast<LastBoss*>(_EnemyObject)->SetSaveState(static_cast<LastBoss::LastBossState>(_EnemyObject->GetNowStateIndex()));
	}
}

void LastBossMagicianState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

	if (EndLocalStateType == EnemyCharacter::State::StartAttack) {
		_ChangeLocalState(EnemyCharacter::State::StartAttack);
	}

	_EnemyObject->ConfigDamageReaction(true, 1);
}
