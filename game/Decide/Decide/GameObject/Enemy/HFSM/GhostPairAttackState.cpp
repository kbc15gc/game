#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "GhostPairAttackState.h"

GhostPairAttackState::GhostPairAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
}


GhostPairAttackState::~GhostPairAttackState()
{
}

void GhostPairAttackState::_EntrySubClass() {

}

void GhostPairAttackState::_Start() {
	// �G�l�~�[�ɂǂ̍U�����s�����𔻒f������B
	// ���G�l�~�[�̍U���p�^�[����I�ʂ���X�e�[�g������Ă��܂��ƁA
	//   �G�l�~�[�̎�ނɉ����ăX�e�[�g�������I�ɑ����Ă��܂����߁A�U���p�^�[���̑I�ʂ͊e���G�l�~�[�ɍs�킹��B
	_EnemyObject->AttackSelect();

	_EnemyObject->ConfigDamageReaction(false);
	_isEndWarp = false;
	_isStartAttackAlready = false;
	_counter = 0.0f;
	_isStartAttack = false;

	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle,0.2f);
}

void GhostPairAttackState::_UpdateSubClass() {
	if (!_isEndWarp) {
		// ���ߏ����ŃS�[�X�g�������B
		// �����̑��߂��ʂ̍s�������Ă���ۂ̂Ƀ^�C�~���O�����킹�邽�߂ɍs���B

		float alpha = _EnemyObject->GetAlpha();

		alpha -= 1.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// ���S�ɓ����ɂȂ����B

			alpha = 0.0f;
			_isEndWarp = true;
		}
		_EnemyObject->SetAlpha(alpha);
	}
	else if ((!_isStartAttackAlready) && _isStartAttack) {
		// ���߂����ׂď������̂ōU���J�n�B

		if (_counter >= _startAttackInterval) {
			// �J�n���Ԃ𑼂̑��߂Ƃ��炷�B
			_ChangeLocalState(EnemyCharacter::State::Attack);
			_isStartAttackAlready = true;
		}
		else {
			_counter += Time::DeltaTime();
		}
	}
}

void GhostPairAttackState::_ExitSubClass(EnemyCharacter::State next) {
}

void GhostPairAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Attack) {
		// �U���I���B

		_EndState();
	}
}
