#include "../EnemyCharacter.h"
#include "EnemyWanderingState.h"


EnemyWanderingState::EnemyWanderingState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWanderingState::~EnemyWanderingState()
{
}

void EnemyWanderingState::_EntrySubClass() {
}

void EnemyWanderingState::_Start() {

}

void EnemyWanderingState::_UpdateSubClass() {

}

void EnemyWanderingState::Exit(EnemyCharacter::State next) {

}

void EnemyWanderingState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// 現在のローカルステートの処理が終了した。

}