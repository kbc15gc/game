#include"stdafx.h"
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

	if (_isOutsideRange) {
		// 前の徘徊処理で範囲外に出た。

		Vector3 dir = _EnemyObject->transform->GetForward();
		dir = dir * -1.0f;	// 向き反転。
		// エネミーを回転させる。
		_EnemyObject->LookAtDirectionInterpolate(dir,0.5f);

		// フラグ初期化。
		_isOutsideRange = false;
	}
	else {
		// 前の徘徊処理で範囲内にいた。

		// ランダムで進む方向を決定する。
		int selectNum = 4;
		float rad = D3DXToRadian(360.0f);
		int rnd = rand() % selectNum;
		rad = rad / (rnd + 1);
		_EnemyObject->RotationAxisInterpolate(Vector3::up, rad,0.5f);
	}

	// パラメータ設定。
	float length = 5.0f;
	float speed = _EnemyObject->GetWalkSpeed();
	_TranslationPalam(_EnemyObject->transform->GetForward(), speed * 5.0f, speed);
}

void EnemyWanderingState::_StartSubClass () {
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(true, 1);
}

void EnemyWanderingState::_UpdateSubClass() {
	// 索敵中なので視野角判定を行う。
	_EnemyObject->SearchView();
}

void EnemyWanderingState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// 現在のローカルステートの処理が終了した。
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// 移動ステート終了。

		//if (_NowLocalState) {
		// 現在のステートはすでに破棄された後なので、配列から取得する。

		_isOutsideRange = static_cast<EnemyTranslationState*>(_EnemyObject->GetMyState()[static_cast<int>(EnemyCharacter::State::Translation)].get())->GetIsOutsideRange();
		// 待機ステートに移行。
		_ChangeLocalState(EnemyCharacter::State::Wait);
		// パラメータ設定。
		static_cast<EnemyWaitState*>(_NowLocalState)->SetInterval(4.5f);
		//}
	}
	else if (EndLocalStateType == EnemyCharacter::State::Wait) {
		// 待機ステート終了。

		// 徘徊ステートいったん終了。
		_EndState();
	}
}

void EnemyWanderingState::_TranslationPalam(const Vector3& dir, float length, float speed) {
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(dir);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(length);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(speed);
	static_cast<EnemyTranslationState*>(_NowLocalState)->SetIsWandering(true);
}