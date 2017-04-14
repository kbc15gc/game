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
	_IsEndState = false;
	_IsFirstUpdate = true;
	// �p����ɂ���ĈقȂ鏈���B
	// ���������z�֐��B
	_EntrySubClass();
}

bool EnemyState::Update() {
	if (_IsFirstUpdate) {
		// �X�e�[�g���؂�ւ���Ă���ŏ��̍X�V�B	
		// �����z�֐��B
		_Start();
		_IsFirstUpdate = false;
	}

	// �p����ɂ���ĈقȂ鏈���B
	// ���������z�֐��B
	_UpdateSubClass();

	if (_NowLocalState) {
		// ���݂̃��[�J���X�e�[�g���ݒ肳��Ă���B
		if (_NowLocalState->Update()) {
			// ���[�J���X�e�[�g�̍X�V�������I�������B

			// �R�[���o�b�N�Ăяo���B
			// ���֐��̓��������͌p����Ŏ����B
			_EndNowLocalState_CallBack(_NowLocalStateIdx);
		}
	}
	return _IsEndState;
}

void EnemyState::_ChangeLocalState(EnemyCharacter::State next) {
	// �G�l�~�[�̎��X�e�[�g�z����Q�ƂŎ󂯎��B
	const vector<unique_ptr<EnemyState>>& enemyState = _EnemyObject->GetMyState();
	
	if (static_cast<int>(next) >= enemyState.size() || static_cast<int>(next) < 0) {
		// �n���ꂽ�������z��̗e�ʂ𒴂��Ă���B
		abort();
	}

	if (_NowLocalState) {
		// ���݂̃X�e�[�g��null�łȂ��B
		// ���݂̃X�e�[�g����Еt���B
		_NowLocalState->Exit(next);
	}

	// �V�����X�e�[�g�ɐ؂�ւ��B
	_NowLocalState = enemyState[static_cast<int>(next)].get();
	_NowLocalState->Entry();

	// ���݂̃X�e�[�g�̓Y������ۑ��B
	_NowLocalStateIdx = next;
}