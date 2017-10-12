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
	// ���G�����B

	// ���䑕�u�������B

	// �S�o�t�����B

	// �����X�e�[�g�Ɉڍs�B
}

void LastBossDownState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossDownState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// �������I��������_�E�����[�V�����őҋ@�B

	// ��ɃN���e�B�J���_���[�W�B

	// ��莞�Ԃŋʍ��X�e�[�g�Ɉڍs�B

	// ���b�菈���B
	_EndState();
}

void LastBossDownState::_ExitSubClass(EnemyCharacter::State next) {
}

void LastBossDownState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
