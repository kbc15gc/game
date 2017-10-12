#include "stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyFallState.h"

EnemyFallState::EnemyFallState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyFallState::~EnemyFallState()
{
}

void EnemyFallState::_EntrySubClass() {
	// 落下中のアニメーション再生。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Fall, 0.1f);
}

void EnemyFallState::_Start() {
}

void EnemyFallState::_UpdateSubClass() {
	if (_EnemyObject->GetIsGround()) {
		// 地面についた。

		_EndState();
	}
}


void EnemyFallState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
