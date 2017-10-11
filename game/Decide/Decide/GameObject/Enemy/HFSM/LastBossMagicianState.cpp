#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossMagicianState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

LastBossMagicianState::LastBossMagicianState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossMagicianState::~LastBossMagicianState()
{
}

void LastBossMagicianState::_EntrySubClass() {
	// 無敵解除。

}

void LastBossMagicianState::_Start() {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(true,30);
}

void LastBossMagicianState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。

	// 確率で攻撃と魔王へのバフを行う。

	// 一定時間で歴史改ざんに移行しステート終了。

	// ※暫定処理。
	_EndState();
}

void LastBossMagicianState::Exit(EnemyCharacter::State next) {
}

void LastBossMagicianState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
