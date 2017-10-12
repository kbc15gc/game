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
	// ���G�����B

	_ChangeLocalState(EnemyCharacter::State::StartAttack);
	_timeCounter = 0.0f;
}

void LastBossMagicianState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true,30);
}

void LastBossMagicianState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B



	if (_timeCounter >= _interval) {
		// ��莞�Ԃŗ��j������Ɉڍs���X�e�[�g�I���B

		_EndState();
	}
	else {
		_timeCounter += Time::DeltaTime();
	}
}

void LastBossMagicianState::_ExitSubClass(EnemyCharacter::State next) {
}

void LastBossMagicianState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// �m���ōU���Ɩ����ւ̃o�t���s���B

	if (EndLocalStateType == EnemyCharacter::State::StartAttack) {
		_ChangeLocalState(EnemyCharacter::State::StartAttack);
	}

}
