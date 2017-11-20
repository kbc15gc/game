#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyState.h"


EnemyState::EnemyState(EnemyCharacter* Object)
{
	_EnemyObject = Object;
}


EnemyState::~EnemyState()
{
}

void EnemyState::Entry() {
	_NowLocalState = nullptr;
	_NowLocalStateIdx = EnemyCharacter::State::None;
	_IsEndState = false;
	_IsFirstUpdate = true;
	// �p����ɂ���ĈقȂ鏈���B
	// ���������z�֐��B
	_EntrySubClass();
}

bool EnemyState::Update() {
	if (!_IsEndState) {
		// ���̃X�e�[�g���I�����Ă��Ȃ��B

		if (_IsFirstUpdate) {
			// �X�e�[�g���؂�ւ���Ă���ŏ��̍X�V�B	
			Start();
			_IsFirstUpdate = false;
		}

		// �p����ɂ���ĈقȂ鏈���B
		// ���������z�֐��B
		_UpdateSubClass();

		if (_NowLocalState) {
			// ���݂̃��[�J���X�e�[�g���ݒ肳��Ă���B
			if (_NowLocalState->Update()) {
				// ���[�J���X�e�[�g�̍X�V�������I�������B

				EnemyCharacter::State work = _NowLocalStateIdx;	// �I�������X�e�[�g��ۑ��B
				// �Ƃ肠�����X�e�[�g��؂�ւ��ĕK���I���������ĂԁB
				_ChangeLocalState(EnemyCharacter::State::None);

				// �R�[���o�b�N�Ăяo���B
				// ���֐��̓��������͌p����Ŏ����B
				_EndNowLocalState_CallBack(work);
			}
		}
	}
	return _IsEndState;
}

void EnemyState::Exit(EnemyCharacter::State next) {

	_ExitSubClass(next);

	if (_NowLocalState) {
		// ���[�J���X�e�[�g���ݒ肳��Ă���B
		_ChangeLocalState(EnemyCharacter::State::None);
	}
	_EnemyObject->SetAnimationSpeed(_saveSpeed);
}

void EnemyState::_ChangeLocalState(EnemyCharacter::State next) {
	// �G�l�~�[�̎��X�e�[�g�z����Q�ƂŎ󂯎��B
	const vector<unique_ptr<EnemyState>>& enemyState = _EnemyObject->GetMyState();
	
	if (next != EnemyCharacter::State::None && static_cast<int>(next) >= static_cast<int>(enemyState.size())) {
		// �n���ꂽ�������z��̗e�ʂ𒴂��Ă���B
		abort();
	}

	if (_NowLocalState) {
		// ���݂̃X�e�[�g��null�łȂ��B
		// ���݂̃X�e�[�g����Еt���B
		_NowLocalState->Exit(next);
	}

	if (next == EnemyCharacter::State::None) {
		// ���̃X�e�[�g���w�肳��Ă��Ȃ��B
		_NowLocalState = nullptr;
	}
	else {
		// �V�����X�e�[�g�ɐ؂�ւ��B
		_NowLocalState = enemyState[static_cast<int>(next)].get();
		_NowLocalState->Entry();
	}

	// ���݂̃X�e�[�g�̓Y������ۑ��B
	_NowLocalStateIdx = next;
}