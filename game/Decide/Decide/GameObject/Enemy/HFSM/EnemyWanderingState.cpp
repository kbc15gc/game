#include "../EnemyCharacter.h"
#include "EnemyWanderingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

EnemyWanderingState::EnemyWanderingState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyWanderingState::~EnemyWanderingState()
{
}

void EnemyWanderingState::_EntrySubClass() {
	// 最初のローカルステートに移行。
	_ChangeLocalState(EnemyCharacter::State::Translation);	// 移動ステートに移行。
	// パラメータ設定。
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(_EnemyObject->transform->GetForward());
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(5.0f);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(1.0f);
}

void EnemyWanderingState::_Start() {

}

void EnemyWanderingState::_UpdateSubClass() {
	// 索敵中なので視野角判定を行う。
	_EnemyObject->SearchView();
}

void EnemyWanderingState::Exit(EnemyCharacter::State next) {

}

void EnemyWanderingState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// 現在のローカルステートの処理が終了した。
	if (now == EnemyCharacter::State::Translation) {
		// 移動ステート終了。

		_ChangeLocalState(EnemyCharacter::State::Wait);		// 待機ステートに移行。
										// パラメータ設定。
		static_cast<EnemyWaitState*>(_NowLocalState)->SetInterval(4.5f);
	}
	else if (now == EnemyCharacter::State::Wait) {
		// 待機ステート終了。

		_ChangeLocalState(EnemyCharacter::State::Translation);	// 移動ステートに移行。
		// パラメータ設定。
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(_EnemyObject->transform->GetForward());
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(5.0f);
		static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(1.0f);
	}
}