#include"stdafx.h"
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
	float InterpolateTime = 0.5f;	// ��Ԏ��ԁB
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Idle, InterpolateTime);
}

void EnemyWaitState::_UpdateSubClass() {
	//if (_TimeCounter >= _Interval) {
	//	// �ݒ肳��Ă������ԑҋ@�����B

	//	// �X�e�[�g�����I���B
	//	_EndState();
	//	return;
	//}
	if (!_EnemyObject->GetIsPlaying()) {
		// �ҋ@�A�j���[�V�����Đ��I���B

		// �X�e�[�g�����I���B
		_EndState();
		return;
	}
	else {
		// ����p������s���B
		_EnemyObject->SearchView();

		//// ���ԉ��Z�B
		//_TimeCounter += Time::DeltaTime();
	}
}

void EnemyWaitState::Exit(EnemyCharacter::State next) {

}

void EnemyWaitState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}