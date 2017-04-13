#include "../EnemyCharacter.h"
#include "EnemyWaitState.h"
#include "fbEngine\CharacterController.h"

EnemyWaitState::EnemyWaitState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWaitState::~EnemyWaitState()
{
}

void EnemyWaitState::_EntrySubClass() {
	_TimeCounter = 0.0f;	// �J�E���^�[�������B
}

void EnemyWaitState::_Start() {
	float InterpolateTime = 0.1f;	// ��Ԏ��ԁB
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, InterpolateTime);
}

void EnemyWaitState::_UpdateSubClass() {
	if (_TimeCounter >= _Interval) {
		// �ݒ肳��Ă������ԑҋ@�����B

		// �X�e�[�g�����I���B
		_EndState();
		return;
	}
	else {
		// ���ԉ��Z�B
		_TimeCounter += Time::DeltaTime();
	}
}

void EnemyWaitState::Exit(EnemyCharacter::State next) {

}

void EnemyWaitState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}