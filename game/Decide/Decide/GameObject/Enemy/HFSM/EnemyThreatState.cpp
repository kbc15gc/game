#include "stdafx.h"
#include "EnemyThreatState.h"

void EnemyThreatState::_EntrySubClass() {
	if (_EnemyObject->GetAnimationNo(EnemyCharacter::AnimationType::Threat) >= 0) {
		// テーブルにアニメーションが設定されている。
		_isNotMotion = false;
	}
	else {
		_isNotMotion = true;
	}
}

void EnemyThreatState::_StartSubClass() {
}

void EnemyThreatState::_UpdateSubClass() {
	if (_isNotMotion || !_EnemyObject->GetIsPlaying()) {
		
		_EndState();
	}
}

void EnemyThreatState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}