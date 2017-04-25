#include "../EnemyCharacter.h"
#include "EnemyDiscoveryState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

EnemyDiscoveryState::EnemyDiscoveryState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyDiscoveryState::~EnemyDiscoveryState()
{
}

void EnemyDiscoveryState::_EntrySubClass() {
	_Player = INSTANCE(GameObjectManager)->FindObject("Player");
	_Speed = 5.0f;	// 移動速度を設定。
}

void EnemyDiscoveryState::_Start() {
	// アニメーション再生。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Dash,0.2f);
}

void EnemyDiscoveryState::_UpdateSubClass() {
	// 暫定処理
	// ※下記のif文条件式にはエネミーの行動範囲外に行ったかの判定を入れる。
	if (true) {
		// 行動範囲内。

		Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
		EnemyToPlayer.y = 0.0f;	// Y軸成分は除く。
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

		// エネミーをプレイヤーに向ける。
		//_EnemyObject->transform->LockAt(_Player);
		_EnemyObject->LookAtObject(*_Player);
	}

	else {
		// 行動範囲から外れた。

		// ※暫定処理。
		// とりあえず初期位置まで戻る。
		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Translation);
		// パラメータ設定。
		Vector3 NowToInitVec = _EnemyObject->GetInitPos() - _EnemyObject->transform->GetPosition();
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetLength(NowToInitVec.Length());
		NowToInitVec.Normalize();
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetDir(NowToInitVec);
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetMoveSpeed(_Speed);
	}
}

void EnemyDiscoveryState::Exit(EnemyCharacter::State next) {

}

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}