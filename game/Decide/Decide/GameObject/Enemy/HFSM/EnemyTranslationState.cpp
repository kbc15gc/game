#include"stdafx.h"
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
	_isWandering = false;
	float InterpolateTime = 0.5f;	// 補間時間。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);
}

void EnemyTranslationState::_Start() {
	//float InterpolateTime = 0.5f;	// 補間時間。
	//_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);

	_InitPos = _EnemyObject->transform->GetLocalPosition();	// 初期位置を保存しておく。
	_isOutsideRange = false;
}

void EnemyTranslationState::_UpdateSubClass() {

	//if (!_isOutsideRange) {
		// 徘徊範囲から外れてない。

		// 初期位置からの距離を算出。
		Vector3 StartToNowVec = _EnemyObject->transform->GetLocalPosition() - _InitPos;
		StartToNowVec.y = 0.0f;
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

			if (_isWandering) {
				// 徘徊ステートから呼ばれている。
				if (_EnemyObject->IsOutsideWandering(moveSpeed)) {
					// 徘徊範囲外に出た。

					_isOutsideRange = true;

					// 今のローカルステートを強制終了。
					_EndState();

					// 移動量を加算しないため処理を終了。
					return;
				}
			}
			// 求めた移動量をエネミーの移動量に加算。
			_EnemyObject->AddMoveSpeed(moveSpeed);
		}
	//}
	//else {
	//	// 徘徊範囲外に出た。

	//}
}


void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	// 現在のローカルステートの処理が終了した。

}