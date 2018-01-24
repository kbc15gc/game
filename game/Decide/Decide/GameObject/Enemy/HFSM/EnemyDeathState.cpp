#include "stdafx.h"
#include "EnemyDeathState.h"
#include "../EnemyCharacter.h"
#include "GameObject\Player\Player.h"

EnemyDeathState::EnemyDeathState(EnemyCharacter* Object):EnemyState(Object) {
	_playAnimation = EnemyCharacter::AnimationType::Death;
}

EnemyDeathState::~EnemyDeathState() {

}

void EnemyDeathState::_EntrySubClass() {
	_isEndAnim = false;
	_timeCounter = 0.0f;

	// エネミー死亡時のドロップ処理。
	_EnemyObject->Drop();
}

void EnemyDeathState::_StartSubClass() {
}

void EnemyDeathState::_UpdateSubClass() {
	if (!_EnemyObject->GetIsPlaying()) {
		// 攻撃モーション一度終了。
		_isEndAnim = true;
	}

	if (_isEndAnim) {
		if (_timeCounter >= _waitTime) {
			// ※死亡ステート終了後はステート切り替えが呼ばれない可能性があるため、Exit関数が呼ばれない可能性がある。
			if (_EnemyObject) {
				// 死亡ステートが終了したのでEnemyManagerの死亡処理を呼ぶ。
				INSTANCE(EnemyManager)->DeathEnemy(_EnemyObject);
			}
			_EndState();
			return;
		}
		_timeCounter += Time::DeltaTime();
		float dc = 65.0f * (_timeCounter / max(0.1f, _waitTime));
		//エネミーが死んだのでディザリングで消滅させる演出。
		_EnemyObject->GetComponent<SkinModel>()->SetDitherCoefficient(dc);
		//消え始めたら当たり判定を消す。
		_EnemyObject->GetComponent<RigidBody>()->SetEnable(false);
		
		_EnemyObject->GetComponent<CCharacterController>()->SetEnable(false);
	}
}

void EnemyDeathState::_ExitSubClass(EnemyCharacter::State next) {
}

void EnemyDeathState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
