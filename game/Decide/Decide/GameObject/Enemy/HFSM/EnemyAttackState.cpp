#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::_EntrySubClass() {
	_attack = _EnemyObject->GetNowSelectAttack();
}

void EnemyAttackState::_Start() {
	if (_attack) {
		// �U���������ݒ肳��Ă���B
		_attack->Entry();	// �������B
	}
	else {
		// �U�������ݒ肳��ĂȂ��B
		abort();
	}
}

void EnemyAttackState::_UpdateSubClass() {
	// �X�V����(�߂�l�͏I��������)�B
	if (_attack->Update()) {
		// �X�V�����I���B
		_EndState();
	}
}

void EnemyAttackState::_ExitSubClass(EnemyCharacter::State next) {
	_attack->Exit();
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
