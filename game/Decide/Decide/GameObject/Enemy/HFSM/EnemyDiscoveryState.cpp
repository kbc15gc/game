#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyDiscoveryState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Player\Player.h"

EnemyDiscoveryState::EnemyDiscoveryState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyDiscoveryState::~EnemyDiscoveryState()
{
}

void EnemyDiscoveryState::_EntrySubClass() {
}

void EnemyDiscoveryState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true, 3);
}

void EnemyDiscoveryState::_UpdateSubClass() {
	// �b�菈��
	_EndState();
}

void EnemyDiscoveryState::Exit(EnemyCharacter::State next) {

}

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}