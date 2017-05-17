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
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Attack,0.2f,1);
}

void EnemyAttackState::_UpdateSubClass() {
	// �U������p�̃R���W���������B
	// ���b�菈���B
	// ���G�l�~�[�͕�����ނ���\�������邽�߁A���[�V�����ɂ���ăR���W���������ʒu��^�C�~���O��ς���K�v������B
	const int eventFrame = 10;
	_EnemyObject->CreateAttackCollision(
		eventFrame,
		_EnemyObject->transform->GetPosition() + _EnemyObject->transform->GetForward() * 0.5f,
		Vector3::zero,
		Vector3::one);

	if (!_EnemyObject->GetIsPlaying()) {
		// �U�����[�V������x�I���B
		_EndState();
	}
}

void EnemyAttackState::Exit(EnemyCharacter::State next) {
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
