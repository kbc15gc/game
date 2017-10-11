#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

LastBossThroneState::LastBossThroneState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossThroneState::~LastBossThroneState()
{
}

void LastBossThroneState::_EntrySubClass() {
	// ���ߏ����B

}

void LastBossThroneState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// ��莞�ԂƊm���Œ����ƃv���C���[�ւ̃f�o�t���s���B

	// ���߂���̂Ƃ��|�����΃X�e�[�g�I���B

	// ���b�菈���B
	_EndState();
}

void LastBossThroneState::Exit(EnemyCharacter::State next) {
	// �ʍ��������B

}

void LastBossThroneState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
