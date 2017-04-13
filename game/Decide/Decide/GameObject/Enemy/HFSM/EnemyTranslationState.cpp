#include "../EnemyCharacter.h"
#include "EnemyTranslationState.h"
#include "fbEngine\CharacterController.h"

EnemyTranslationState::EnemyTranslationState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyTranslationState::~EnemyTranslationState()
{
}

void EnemyTranslationState::_EntrySubClass() {
}

void EnemyTranslationState::_Start() {
	float InterpolateTime = 0.1f;	// ��Ԏ��ԁB
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);
	_InitPos = _EnemyObject->transform->GetLocalPosition();	// �����ʒu��ۑ����Ă����B
}

void EnemyTranslationState::_UpdateSubClass() {
	// �����ʒu����̋������Z�o�B
	Vector3 StartToNowVec = _EnemyObject->transform->GetLocalPosition() - _InitPos;
	float Length = StartToNowVec.Length();

	if (Length >= _MoveLength) {
		// �w�肳��Ă����ړ��ʂ𒴂����B

		// ���̃X�e�[�g�̏������I������B
		_EndState();
		return;
	}
	else {
		// �w�肳�ꂽ�ړ��ʂ܂ňړ����Ă��Ȃ��B

		Vector3 moveSpeed = Vector3::zero;
		moveSpeed = this->_Direction * this->_MoveSpeed;	// �����Ɉړ��ʂ��|����B

		// ���߂��ړ��ʂ��G�l�~�[�̈ړ��ʂɉ��Z�B
		_EnemyObject->AddMoveSpeed(moveSpeed);
	}
}

void EnemyTranslationState::Exit(EnemyCharacter::State next) {

}

void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}