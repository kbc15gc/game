#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyTranslationState.h"
#include "fbEngine\CharacterController.h"

EnemyTranslationState::EnemyTranslationState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::Walk;
	_interpolate = 0.5f;
	_loopNum = -1;
}


EnemyTranslationState::~EnemyTranslationState()
{
}

void EnemyTranslationState::_EntrySubClass() {
	_isWandering = false;
}

void EnemyTranslationState::_StartSubClass() {

	_InitPos = _EnemyObject->transform->GetPosition();	// �����ʒu��ۑ����Ă����B
	_isOutsideRange = false;
}

void EnemyTranslationState::_UpdateSubClass() {

	//if (!_isOutsideRange) {
		// �p�j�͈͂���O��ĂȂ��B


	// �����ʒu����̋������Z�o�B
	Vector3 StartToNowVec = _EnemyObject->transform->GetPosition() - _InitPos;
	StartToNowVec.y = 0.0f;
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

		if (_isWandering) {
			// �p�j�X�e�[�g����Ă΂�Ă���B
			if (_EnemyObject->IsOutsideWandering(moveSpeed)) {
				// �p�j�͈͊O�ɏo���B

				_isOutsideRange = true;

				// ���̃��[�J���X�e�[�g�������I���B
				_EndState();

				// �ړ��ʂ����Z���Ȃ����ߏ������I���B
				return;
			}
		}
		// ���߂��ړ��ʂ��G�l�~�[�̈ړ��ʂɉ��Z�B
		_EnemyObject->AddMoveSpeed(moveSpeed);
	}
}


void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}