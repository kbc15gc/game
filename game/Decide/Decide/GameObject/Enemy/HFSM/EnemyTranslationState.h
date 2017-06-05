#pragma once

#include "EnemyState.h"

class EnemyCharacter;

// エネミー直進ステート。
class EnemyTranslationState : public EnemyState {
public:
	EnemyTranslationState(EnemyCharacter* Object);
	~EnemyTranslationState();


	void Exit(EnemyCharacter::State next)override;


	// 移動する方向を指定。
	inline void SetDir(const Vector3& dir) {
		_Direction = dir;
		// 正規化。
		_Direction.Normalize();
	}

	// 移動量設定。
	inline void SetLength(const float length) {
		_MoveLength = length;
	}

	// 移動速度設定(毎秒)。
	inline void SetMoveSpeed(const float speed) {
		_MoveSpeed = speed;
	}

	// この関数でtrueを設定すると、徘徊範囲外に出たらステートを終了するようになる。
	inline void SetIsWandering(bool flg) {
		_isWandering = flg;
	}

	// 徘徊範囲外に出たか返却
	inline bool GetIsOutsideRange() const {
		return _isOutsideRange;
	}
private:
	void _EntrySubClass()override;

	void _Start()override;

	void _UpdateSubClass()override;

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);

private:
	Vector3 _Direction;	// 進む方向。
	float _MoveLength;	// 移動量。
	float _MoveSpeed;	// 移動するスピード(毎秒)。
	bool _isOutsideRange = false;
	// 以下作業用。
	Vector3 _InitPos;	// 初期位置。
	bool _isWandering = false;	// 徘徊中の直進か。
};