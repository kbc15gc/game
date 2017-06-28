#include "stdafx.h"
#include "EnemyDeathState.h"
#include "../EnemyCharacter.h"

EnemyDeathState::EnemyDeathState(EnemyCharacter* Object):EnemyState(Object) {

}

EnemyDeathState::~EnemyDeathState() {

}

void EnemyDeathState::_EntrySubClass() {
	_isEndAnim = false;
	_timeCounter = 0.0f;
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Death,0.5f);
}

void EnemyDeathState::_Start() {

}

void EnemyDeathState::_UpdateSubClass() {
	if (!_EnemyObject->GetIsPlaying()) {
		// 攻撃モーション一度終了。
		_isEndAnim = true;
	}

	if (_isEndAnim) {
		if (_timeCounter >= _waitTime) {
			_EndState();
			return;
		}
		_timeCounter += Time::DeltaTime();
	}
}

void EnemyDeathState::Exit(EnemyCharacter::State next) {
	if (_EnemyObject) {
		// 死亡ステートが終了したのでEnemyManagerの死亡処理を呼ぶ。
		INSTANCE(GameObjectManager)->AddRemoveList(_EnemyObject);
	}
}

void EnemyDeathState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
