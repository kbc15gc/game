#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossMagicianState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\LastBoss.h"
#include "Scene\GameScene.h"

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
	_EnemyObject->ConfigDamageReaction(true, _EnemyObject->GetDamageMotionRandNum());
}

void LastBossMagicianState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。
	if (_EnemyObject->IsOutsideDiscovery()) {
		static_cast<LastBoss*>(_EnemyObject)->BattleEnd();
		return;
	}

	//if (_timeCounter >= _interval) {
	//	// 一定時間で歴史改ざんに移行しステート終了。

	//	_EndState();
	//}
	//else {
	//	_timeCounter += Time::DeltaTime();
	//}
}

void LastBossMagicianState::_ExitSubClass(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Damage) {
		static_cast<LastBoss*>(_EnemyObject)->SetSaveState(static_cast<LastBoss::LastBossState>(_EnemyObject->GetNowStateIndex()));
	}/*else if(next != EnemyCharacter::State::Death){
		_EnemyObject->EnemyStopSound(static_cast<EnemyCharacter::SoundIndex>(LastBoss::LastBossSoundIndex::Battle2));
		static_cast<GameScene*>(INSTANCE(SceneManager)->GetNowScene())->ResetBGMIndex();
	}*/
}

void LastBossMagicianState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

	if (EndLocalStateType == EnemyCharacter::State::StartAttack) {
		_ChangeLocalState(EnemyCharacter::State::StartAttack);
	}

	//_EnemyObject->ConfigDamageReaction(true, 1);
}
