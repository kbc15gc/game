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
	_Player = GameObjectManager::FindObject("Player");
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

		// 今の向きとプレイヤーまでの向きの角度を算出。
		Vector3 EnemyDir = _EnemyObject->transform->GetForward();
		EnemyDir.y = 0.0f;	// Y軸成分は除く。
		EnemyDir.Normalize();
		float dot = EnemyToPlayer.Dot(EnemyDir);
		float rad = acosf(dot);	

		if (rad > 0.001f) {
			// プレイヤーのほうを向いていない。

			// 回転方向算出。
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross,&D3DXVECTOR3(EnemyDir.x, EnemyDir.y, EnemyDir.z),&D3DXVECTOR3(EnemyToPlayer.x, EnemyToPlayer.y, EnemyToPlayer.z));
			//Vector3 cross = EnemyToPlayer;
			//cross.Cross(EnemyDir);
			//cross.Normalize();

			// テスト。
			static float time = 0.0f;
			time += Time::DeltaTime();
			float kakudo = D3DXToDegree(rad);

			if (time >= 2.0f) {
				time = 0.0f;
				OutputDebugString(_T("あ"));
			}
			if (rad > D3DXToRadian(180.0f)) {
				// 半回転より大きい角度を取得した。

				// 小さいほうの角度を算出。
				rad = D3DXToRadian(360.0f) - rad;
				cross *= -1.0f;
			}

			// クォータニオンで回転。
			Quaternion quat = Quaternion::Identity;
			quat.SetRotation(cross, rad);
			Quaternion Rotation = _EnemyObject->transform->GetRotation();
			quat.Multiply(Rotation);
			_EnemyObject->transform->SetLocalAngle(quat.GetAngle());
			//_EnemyObject->transform->SetRotation(quat);
		}

		//if (rad > 0.001f) {
		//	Vector3 Euler = _EnemyObject->transform->GetAngle();	// オイラー角で取得。
		//	if (/*AngleDir*/dot < 0.0f) {
		//		OutputDebugString(_T("マイナス\n"));
		//	}
		//	Euler.y = Euler.y + (rad * AngleDir);
		//	_EnemyObject->transform->SetAngle(Euler);
		//}
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

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
}

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}