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
	_attack = _EnemyObject->GetNowSelectAttack();
}

void EnemyAttackState::_Start() {
	if (_attack) {
		// �U���������ݒ肳��Ă���B

		_attack->Entry();	// �������B
		if (_attack->GetAnimationType() >= 0) {
			// �Đ�����A�j���[�V�����ԍ����ݒ肳��Ă���B
			_EnemyObject->PlayAnimation_OriginIndex(_attack->GetAnimationType(), _attack->GetInterpolate(), _attack->GetAnimationLoopNum(),_attack->GetPlayEventNo());
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

void EnemyAttackState::_ExitSubClass(EnemyCharacter::State next) {
	_attack->Exit();
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
