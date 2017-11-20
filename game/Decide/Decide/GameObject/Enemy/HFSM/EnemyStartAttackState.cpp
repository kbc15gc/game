#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "EnemyAttackState.h"
#include "GameObject\Enemy\HFSM\EnemyChaceState.h"

EnemyStartAttackState::EnemyStartAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
}


EnemyStartAttackState::~EnemyStartAttackState()
{
}

void EnemyStartAttackState::_EntrySubClass() {

}

void EnemyStartAttackState::_StartSubClass() {
	// �G�l�~�[�ɂǂ̍U�����s�����𔻒f������B
	// ���G�l�~�[�̍U���p�^�[����I�ʂ���X�e�[�g������Ă��܂��ƁA
	//   �G�l�~�[�̎�ނɉ����ăX�e�[�g�������I�ɑ����Ă��܂����߁A�U���p�^�[���̑I�ʂ͊e���G�l�~�[�ɍs�킹��B
	_EnemyObject->AttackSelect();

	// �U���\�͈͂܂ŒǐՂ�����B
	_ChangeLocalState(EnemyCharacter::State::Chace);
	static_cast<EnemyChaceState*>(_NowLocalState)->SetEndRange(_EnemyObject->GetNowSelectAttackRange());
}

void EnemyStartAttackState::_UpdateSubClass() {

}

void EnemyStartAttackState::_ExitSubClass(EnemyCharacter::State next) {
}

void EnemyStartAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Chace) {
		// �U���͈͓��B

		// �U���J�n�B
		_ChangeLocalState(EnemyCharacter::State::Attack);
	}
	else if (EndLocalStateType == EnemyCharacter::State::Attack) {
		// �U���I���B

		_EndState();
	}
}
