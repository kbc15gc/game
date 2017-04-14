#include "../EnemyCharacter.h"
#include "EnemyTranslationState.h"
#include "fbEngine\CharacterController.h"

EnemyTranslationState::EnemyTranslationState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyTranslationState::~EnemyTranslationState()
{
}

void EnemyTranslationState::_EntrySubClass() {
}

void EnemyTranslationState::_Start() {
	float InterpolateTime = 0.1f;	// 補間時間。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);
	_InitPos = _EnemyObject->transform->GetLocalPosition();	// 初期位置を保存しておく。
}

void EnemyTranslationState::_UpdateSubClass() {
	// 初期位置からの距離を算出。
	Vector3 StartToNowVec = _EnemyObject->transform->GetLocalPosition() - _InitPos;
	float Length = StartToNowVec.Length();

	if (Length >= _MoveLength) {
		// 指定されていた移動量を超えた。

		// このステートの処理を終了する。
		_EndState();
		return;
	}
	else {
		// 指定された移動量まで移動していない。

		Vector3 moveSpeed = Vector3::zero;
		moveSpeed = this->_Direction * this->_MoveSpeed;	// 方向に移動量を掛ける。

		// 求めた移動量をエネミーの移動量に加算。
		_EnemyObject->AddMoveSpeed(moveSpeed);
	}
}

void EnemyTranslationState::Exit(EnemyCharacter::State next) {

}

void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// 現在のローカルステートの処理が終了した。

}