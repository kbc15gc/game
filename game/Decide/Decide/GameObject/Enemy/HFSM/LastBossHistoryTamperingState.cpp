#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossHistoryTamperingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\LastBoss.h"

LastBossHistoryTamperingState::LastBossHistoryTamperingState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossHistoryTamperingState::~LastBossHistoryTamperingState()
{
}

void LastBossHistoryTamperingState::_EntrySubClass() {
	// ���G�����B
	
	// ���j�������B
}

void LastBossHistoryTamperingState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossHistoryTamperingState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// ���䑕�u�𔭓��������̂��A�ɑ�U�������B

	// �喂�@���������ƃX�e�[�g�I���B

	// �喂�@���L�����Z�������ƃ_�E���Ɉڍs�B
	_EnemyObject->ChangeStateRequest(static_cast<EnemyCharacter::State>(LastBoss::LastBossState::LastBossDown));
	
	// ���b�菈���B
	_EndState();
}

void LastBossHistoryTamperingState::_ExitSubClass(EnemyCharacter::State next) {
	// ���j���������B

}

void LastBossHistoryTamperingState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
