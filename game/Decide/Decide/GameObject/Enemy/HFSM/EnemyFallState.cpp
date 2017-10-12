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
	// �������̃A�j���[�V�����Đ��B
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Fall, 0.1f);
}

void EnemyFallState::_Start() {
}

void EnemyFallState::_UpdateSubClass() {
	if (_EnemyObject->GetIsGround()) {
		// �n�ʂɂ����B

		_EndState();
	}
}


void EnemyFallState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
