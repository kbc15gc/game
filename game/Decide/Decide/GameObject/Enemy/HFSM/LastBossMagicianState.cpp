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

}

void LastBossMagicianState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true,30);
}

void LastBossMagicianState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// �m���ōU���Ɩ����ւ̃o�t���s���B

	// ��莞�Ԃŗ��j������Ɉڍs���X�e�[�g�I���B

	// ���b�菈���B
	_EndState();
}

void LastBossMagicianState::Exit(EnemyCharacter::State next) {
}

void LastBossMagicianState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
