#include "stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyFallState.h"

EnemyFallState::EnemyFallState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::Fall;
	_interpolate = 0.1f;
	_loopNum = -1;
}


EnemyFallState::~EnemyFallState()
{
}

void EnemyFallState::_EntrySubClass() {
}

void EnemyFallState::_StartSubClass() {
}

void EnemyFallState::_UpdateSubClass() {
	if (_EnemyObject->GetIsGround()) {
		// ínñ Ç…Ç¬Ç¢ÇΩÅB

		_EndState();
	}
}


void EnemyFallState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
