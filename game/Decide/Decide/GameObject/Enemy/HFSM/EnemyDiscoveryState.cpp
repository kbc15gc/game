#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyDiscoveryState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Player\Player.h"

EnemyDiscoveryState::EnemyDiscoveryState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyDiscoveryState::~EnemyDiscoveryState()
{
}

void EnemyDiscoveryState::_EntrySubClass() {
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_Speed = _EnemyObject->GetDashSpeed();	// 移動速度を設定。
	_isOutside = false;
}

void EnemyDiscoveryState::_Start() {
	// アニメーション再生。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Dash,0.2f);
	// のけぞり設定。
	_EnemyObject->ConfigDamageReaction(true, 3);
}

void EnemyDiscoveryState::_UpdateSubClass() {
	// 暫定処理
	if (_Player->GetState() == Player::State::Death) {
		// プレイヤーが死んでいる。

		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Wandering);
	}
	else if (!_isOutside) {
		if (!_EnemyObject->IsOutsideWandering()) {
			// 行動範囲内。

			// 追跡処理。
			// エネミーをプレイヤーに向ける。
			_EnemyObject->LookAtObject(*_Player);

			Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			if (EnemyToPlayer.Length() <= _EnemyObject->GetAttackRange()) {
				// 攻撃範囲に入った。

				// ステート終了。
				_EndState();
				return;
			}

			EnemyToPlayer.Normalize();
			// 移動量計算。
			Vector3 moveSpeeed = EnemyToPlayer * _Speed;

			// 算出した移動をエネミーに加算。
			_EnemyObject->AddMoveSpeed(moveSpeeed);
		}
		else {
			// 行動範囲から外れた。

			_isOutside = true;

			// とりあえず初期位置まで戻る。
			_ChangeLocalState(EnemyCharacter::State::Translation);
			// パラメータ設定。
			Vector3 NowToInitVec = _EnemyObject->GetInitPos() - _EnemyObject->transform->GetPosition();
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(NowToInitVec.Length());
			NowToInitVec.Normalize();
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(NowToInitVec);
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(_Speed);

			// エネミーを進む方向に向ける。
			_EnemyObject->LookAtDirection(NowToInitVec);
		}
	}
	//else {
	//	// ※暫定処理。
	//}
}

void EnemyDiscoveryState::Exit(EnemyCharacter::State next) {

}

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// 初期位置に戻った。

		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Wandering);
	}
}

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}