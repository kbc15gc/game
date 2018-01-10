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

	Vector3 dir = _EnemyObject->transform->GetForward();

	if (_isOutsideRange) {
		// 前の徘徊処理で範囲外に出た。

		dir = dir * -1.0f;	// 向き反転。
		// エネミーを回転させる。
		_EnemyObject->LookAtDirectionInterpolate(dir, 0.5f);

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

		// より少ない回転量で済む方向に変換。
		rad -= D3DXToRadian(180.0f);
		
		float interpolate = 0.5f;
		if (fabsf(rad) >= 180.0f) {
			// 補間時間を長くする。
			interpolate *= 2.0f;
		}

		_EnemyObject->RotationAxisInterpolate(Vector3::up, rad, interpolate);
		Quaternion rot;
		rot = Quaternion::Identity;
		rot.SetRotation(Vector3::up, rad);
		dir = rot.RotationVector3(dir);
	}

	// パラメータ設定。
	float length = 5.0f;
	float speed = _EnemyObject->GetWalkSpeed();
	_TranslationPalam(dir, speed * 5.0f, speed);
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
	static_cast<EnemyTranslationState*>(_NowLocalState)->CustamParameter(EnemyCharacter::AnimationType::Walk, _NowLocalState->GetInterpolate(), -1, _NowLocalState->GetEventNo(), 1.0f);
}