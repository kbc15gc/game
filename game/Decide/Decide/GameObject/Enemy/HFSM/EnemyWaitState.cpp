#include "../EnemyCharacter.h"
#include "EnemyWaitState.h"
#include "fbEngine\CharacterController.h"

EnemyWaitState::EnemyWaitState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWaitState::~EnemyWaitState()
{
}

void EnemyWaitState::_EntrySubClass() {
	_TimeCounter = 0.0f;	// カウンター初期化。
}

void EnemyWaitState::_Start() {
	float InterpolateTime = 0.1f;	// 補間時間。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, InterpolateTime);
}

void EnemyWaitState::_UpdateSubClass() {
	if (_TimeCounter >= _Interval) {
		// 設定されていた時間待機した。

		// ステート処理終了。
		_EndState();
		return;
	}
	else {
		// 時間加算。
		_TimeCounter += Time::DeltaTime();
	}
}

void EnemyWaitState::Exit(EnemyCharacter::State next) {

}

void EnemyWaitState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// 現在のローカルステートの処理が終了した。

}