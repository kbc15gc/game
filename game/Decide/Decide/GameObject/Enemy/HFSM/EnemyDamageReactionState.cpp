#include "stdafx.h"
#include "GameObject\Enemy\HFSM\EnemyDamageReactionState.h"


void EnemyDamageReactionState::_EntrySubClass() {
}

void EnemyDamageReactionState::_UpdateSubClass() 
{
	if (!_EnemyObject->GetIsPlaying()) {
		// ダメージモーション一度終了。
		_EndState();
	}
}

void EnemyDamageReactionState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
