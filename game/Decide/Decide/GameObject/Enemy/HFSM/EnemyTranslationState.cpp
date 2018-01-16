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
	_isStartTrans = true;
}

void EnemyTranslationState::_StartSubClass() {

	_InitPos = _EnemyObject->transform->GetPosition();	// �����ʒu��ۑ����Ă����B
	_isOutsideRange = false;
}

void EnemyTranslationState::_UpdateSubClass() {

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

		if (!_isStartTrans && _EnemyObject->GetMoveSpeedExcute().Length() <= 0.01f) {
			// �ړ����n�߂Ă���ǂȂǂɓ˂�����ł�������������Ĉړ����Ă��Ȃ��B
			//if (_EnemyObject->GetIsOnWall()) {
			//	// �ǂɏՓ˂��Ă���B

				_EndState();
			//}
		}

		// ���߂��ړ��ʂ��G�l�~�[�̈ړ��ʂɉ��Z�B
		_EnemyObject->AddMoveSpeed(moveSpeed);
	}

	_isStartTrans = false;
}


void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}