#include "stdafx.h"
#include "GameObject\Enemy\HFSM\EnemyDamageReactionState.h"


void EnemyDamageReactionState::_EntrySubClass() {
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Damage, 0.2f);
}

void EnemyDamageReactionState::_UpdateSubClass() 
{
	if (!_EnemyObject->GetIsPlaying()) {
		// �_���[�W���[�V������x�I���B
		_EndState();
	}
}

void EnemyDamageReactionState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
