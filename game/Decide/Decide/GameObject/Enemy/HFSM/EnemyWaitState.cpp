#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyWaitState.h"
#include "fbEngine\CharacterController.h"

EnemyWaitState::EnemyWaitState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::Idle;
	_interpolate = 0.5f;
}


EnemyWaitState::~EnemyWaitState()
{
}

void EnemyWaitState::_EntrySubClass() {
	_TimeCounter = 0.0f;	// カウンター初期化。
}

void EnemyWaitState::_StartSubClass() {
}

void EnemyWaitState::_UpdateSubClass() {
	//if (_TimeCounter >= _Interval) {
	//	// 設定されていた時間待機した。

	//	// ステート処理終了。
	//	_EndState();
	//	return;
	//}
	if (!_EnemyObject->GetIsPlaying()) {
		// 待機アニメーション再生終了。

		// ステート処理終了。
		_EndState();
		return;
	}
	else {
		// 視野角判定を行う。
		_EnemyObject->SearchView();

		//// 時間加算。
		//_TimeCounter += Time::DeltaTime();
	}
}

void EnemyWaitState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// 現在のローカルステートの処理が終了した。

}