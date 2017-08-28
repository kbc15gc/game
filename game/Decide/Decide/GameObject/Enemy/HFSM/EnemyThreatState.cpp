#include "stdafx.h"
#include "EnemyThreatState.h"

void EnemyThreatState::_EntrySubClass() {
	if (_EnemyObject->GetAnimationData(EnemyCharacter::AnimationType::Threat).No >= 0) {
		// �e�[�u���ɃA�j���[�V�������ݒ肳��Ă���B
		_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Threat, 0.5f);
	}
	//else {
	//	_EndState();
	//}
}

void EnemyThreatState::Start() {
	// �Ă���
	//if (_EnemyObject->GetAnimationData(EnemyCharacter::AnimationType::Threat).No >= 0) {
	//	// �e�[�u���ɃA�j���[�V�������ݒ肳��Ă���B
	//	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Threat, 0.5f);
	//}
}

void EnemyThreatState::_UpdateSubClass() {
	if (!_EnemyObject->GetIsPlaying()) {
		_EndState();
	}
}

void EnemyThreatState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}