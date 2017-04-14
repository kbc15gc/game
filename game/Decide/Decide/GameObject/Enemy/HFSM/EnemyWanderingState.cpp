#include "../EnemyCharacter.h"
#include "EnemyWanderingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

EnemyWanderingState::EnemyWanderingState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWanderingState::~EnemyWanderingState()
{
}

void EnemyWanderingState::_EntrySubClass() {
	// �ŏ��̃��[�J���X�e�[�g�Ɉڍs�B
	_ChangeLocalState(EnemyCharacter::State::Translation);	// �ړ��X�e�[�g�Ɉڍs�B
	// �p�����[�^�ݒ�B
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(_EnemyObject->transform->GetForward());
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(5.0f);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(1.0f);
}

void EnemyWanderingState::_Start() {

}

void EnemyWanderingState::_UpdateSubClass() {
	// ���G���Ȃ̂Ŏ���p������s���B
	_EnemyObject->SearchView();
}

void EnemyWanderingState::Exit(EnemyCharacter::State next) {

}

void EnemyWanderingState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B
	if (now == EnemyCharacter::State::Translation) {
		// �ړ��X�e�[�g�I���B

		_ChangeLocalState(EnemyCharacter::State::Wait);		// �ҋ@�X�e�[�g�Ɉڍs�B
										// �p�����[�^�ݒ�B
		static_cast<EnemyWaitState*>(_NowLocalState)->SetInterval(4.5f);
	}
	else if (now == EnemyCharacter::State::Wait) {
		// �ҋ@�X�e�[�g�I���B

		_ChangeLocalState(EnemyCharacter::State::Translation);	// �ړ��X�e�[�g�Ɉڍs�B
		// �p�����[�^�ݒ�B
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(_EnemyObject->transform->GetForward());
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(5.0f);
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(1.0f);
	}
}