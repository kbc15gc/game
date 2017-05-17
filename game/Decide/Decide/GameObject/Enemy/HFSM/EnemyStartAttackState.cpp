#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyStartAttackState.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"

EnemyStartAttackState::EnemyStartAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
}


EnemyStartAttackState::~EnemyStartAttackState()
{
}

void EnemyStartAttackState::_EntrySubClass() {

}

void EnemyStartAttackState::_Start() {
	Vector3 EnemyToPlayer(_Player->transform->GetPosition() - _EnemyObject->transform->GetPosition());

	if (false) {
		// �v���C���[�����S�����B
	}
	else if (EnemyToPlayer.Length() >= _EnemyObject->GetAttackRange()) {
		// �v���C���[���U���͈͊O�ɗ��E�����B

		// �ĂђǐՂ�����B
		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Discovery);
	}
	else {
		// �v���C���[���U���\�B

		// �G�l�~�[�ɂǂ̍U�����s�����𔻒f������B
		// ���G�l�~�[�̍U���p�^�[����I�ʂ���X�e�[�g������Ă��܂��ƁA
		//   �G�l�~�[�̎�ނɉ����ăX�e�[�g�������I�ɑ����Ă��܂����߁A�U���p�^�[���̑I�ʂ͊e���G�l�~�[�ɍs�킹��B
		_ChangeLocalState(_EnemyObject->AttackSelect());
	}
}

void EnemyStartAttackState::_UpdateSubClass() {

}

void EnemyStartAttackState::Exit(EnemyCharacter::State next) {
}

void EnemyStartAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// �U���I���B

	_EndState();
}
