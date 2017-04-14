
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
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Attack,0.2f);
}

void EnemyAttackState::_UpdateSubClass() {
}

void EnemyAttackState::Exit(EnemyCharacter::State next) {
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// çUåÇèIóπÅB

	_EndState();
}
