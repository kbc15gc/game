#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossDownState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

LastBossDownState::LastBossDownState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossDownState::~LastBossDownState()
{
}

void LastBossDownState::_EntrySubClass() {
	// 無敵解除。

	// 舞台装置を消す。

	// 全バフ解除。

	// 落下ステートに移行。
}

void LastBossDownState::_Start() {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossDownState::_UpdateSubClass() {
	// 常にプレイヤーと距離判定し、バトル範囲外に出たら初期ステートに戻す。

	// 落下が終了したらダウンモーションで待機。

	// 常にクリティカルダメージ。

	// 一定時間で玉座ステートに移行。

	// ※暫定処理。
	_EndState();
}

void LastBossDownState::_ExitSubClass(EnemyCharacter::State next) {
}

void LastBossDownState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
