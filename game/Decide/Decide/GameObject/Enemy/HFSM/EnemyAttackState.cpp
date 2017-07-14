#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::_EntrySubClass() {

}

void EnemyAttackState::_Start() {
	if (_attack) {
		// �U���������ݒ肳��Ă���B

		_attack->Start();	// �������B
		if (_attack->GetAnimationType() >= 0) {
			// �Đ�����A�j���[�V�����ԍ����ݒ肳��Ă���B
			if (_attack->GetAnimationLoopNum() < 0) {
				// �������[�v�Đ��B
			}
			else if(_attack->GetAnimationLoopNum() > 0){
				// �w��񐔃��[�v�Đ��B
				_EnemyObject->PlayAnimation_OriginIndex(_attack->GetAnimationType(), _attack->GetInterpolate(), _attack->GetAnimationLoopNum());
			}
			else {
				// �Đ��񐔂�0���ݒ肳�ꂽ�B
				abort();
			}
		}
	}
	else {
		// �U�������ݒ肳��ĂȂ��B
		abort();
	}
}

void EnemyAttackState::_UpdateSubClass() {
	_attack->SetIsPlaying(_EnemyObject->GetIsPlaying());	// �A�j���[�V�����̍X�V�󋵂�ʒm�B

	// �X�V����(�߂�l�͏I��������)�B
	if (_attack->Update()) {
		// �X�V�����I���B
		_EndState();
	}
}

void EnemyAttackState::Exit(EnemyCharacter::State next) {
	_attack->Exit();
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
