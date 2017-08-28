#include "stdafx.h"
#include "EnemyDeathState.h"
#include "../EnemyCharacter.h"
#include "GameObject\History\Chip.h"
#include "GameObject\Player\Player.h"

EnemyDeathState::EnemyDeathState(EnemyCharacter* Object):EnemyState(Object) {
	//プレイヤー
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

EnemyDeathState::~EnemyDeathState() {

}

void EnemyDeathState::_EntrySubClass() {
	_isEndAnim = false;
	_timeCounter = 0.0f;
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Death,0.5f);

	Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
	chip->SetDropChipID(ChipID(1), _EnemyObject->transform->GetPosition());

	_Player->TakeDrop(_EnemyObject->GetDropEXP(), _EnemyObject->GetDropMoney());
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
			// ※死亡ステート終了後はステート切り替えが呼ばれない可能性があるため、Exit関数が呼ばれない可能性がある。
			if (_EnemyObject) {
				// 死亡ステートが終了したのでEnemyManagerの死亡処理を呼ぶ。
				INSTANCE(EnemyManager)->DeathEnemy(_EnemyObject);
			}
			_EndState();
			return;
		}
		_timeCounter += Time::DeltaTime();
	}
}

void EnemyDeathState::Exit(EnemyCharacter::State next) {
}

void EnemyDeathState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {

}
