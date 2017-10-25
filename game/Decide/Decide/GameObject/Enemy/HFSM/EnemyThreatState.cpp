#include "stdafx.h"
#include "EnemyThreatState.h"

void EnemyThreatState::_EntrySubClass() {
	if (_EnemyObject->GetAnimationNo(EnemyCharacter::AnimationType::Threat) >= 0) {
		// テーブルにアニメーションが設定されている。
		_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Threat, 0.5f);
		_isNotMotion = false;
	}
	else {
		_isNotMotion = true;
	}
}

void EnemyThreatState::Start() {
}

void EnemyThreatState::_UpdateSubClass() {
	if (_isNotMotion || !_EnemyObject->GetIsPlaying()) {
		
		_EndState();
	}
}

void EnemyThreatState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}