#include "stdafx.h"
#include "EnemyDeathState.h"
#include "../EnemyCharacter.h"
#include "GameObject\Player\Player.h"

EnemyDeathState::EnemyDeathState(EnemyCharacter* Object):EnemyState(Object) {
}

EnemyDeathState::~EnemyDeathState() {

}

void EnemyDeathState::_EntrySubClass() {
	_isEndAnim = false;
	_timeCounter = 0.0f;
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Death,0.5f);

	// �G�l�~�[���S���̃h���b�v�����B
	_EnemyObject->Drop();
}

void EnemyDeathState::_Start() {
}

void EnemyDeathState::_UpdateSubClass() {
	if (!_EnemyObject->GetIsPlaying()) {
		// �U�����[�V������x�I���B
		_isEndAnim = true;
	}

	if (_isEndAnim) {
		if (_timeCounter >= _waitTime) {
			// �����S�X�e�[�g�I����̓X�e�[�g�؂�ւ����Ă΂�Ȃ��\�������邽�߁AExit�֐����Ă΂�Ȃ��\��������B
			if (_EnemyObject) {
				// ���S�X�e�[�g���I�������̂�EnemyManager�̎��S�������ĂԁB
				INSTANCE(EnemyManager)->DeathEnemy(_EnemyObject);
			}
			_EndState();
			return;
		}
		_timeCounter += Time::DeltaTime();
	}
}

void EnemyDeathState::_ExitSubClass(EnemyCharacter::State next) {
}

void EnemyDeathState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
