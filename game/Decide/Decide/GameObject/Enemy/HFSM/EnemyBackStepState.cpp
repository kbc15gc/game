#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyBackStepState.h"
#include "fbEngine\CharacterController.h"

EnemyBackStepState::EnemyBackStepState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::BackStep;
}


EnemyBackStepState::~EnemyBackStepState()
{
}

void EnemyBackStepState::_EntrySubClass() {
	_isBack = false;
}

void EnemyBackStepState::_StartSubClass() {
	_isAddPower = true;
	_nowBackPower = 0.0f;
}

void EnemyBackStepState::_UpdateSubClass() {

	if (_counter >= _interval) {
		_isBack = true;
	}

	if (_isBack) {
		if (_isAddPower) {
			if (_nowBackPower >= _maxPower) {
				_nowBackPower = _maxPower;
				_isAddPower = false;
			}
			else {
				_nowBackPower += _addPower * Time::DeltaTime();
			}
		}
		else {
			if (_nowBackPower <= 0.0f) {
				_nowBackPower = 0.0f;
				_EndState();
			}
			else {
				_nowBackPower -= _addPower * Time::DeltaTime();
			}
		}
		_EnemyObject->AddMoveSpeed(_Direction * _nowBackPower);
	}

}


void EnemyBackStepState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// 現在のローカルステートの処理が終了した。

}