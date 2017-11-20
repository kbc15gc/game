#pragma once

#include "EnemyState.h"

// エネミーバックステップステート。
class EnemyBackStepState : public EnemyState {
public:
	EnemyBackStepState(EnemyCharacter* Object);
	~EnemyBackStepState();

	// 移動する方向を指定。
	inline void SetDirection(const Vector3& dir) {
		_Direction = dir;
		// 正規化。
		_Direction.Normalize();
	}
	inline void SetAddPower(float add) {
		_addPower = add;
	}
	inline void SetMaxPower(float max) {
		_maxPower = max;
	}

private:
	void _EntrySubClass()override;

	void _StartSubClass()override;

	void _UpdateSubClass()override;

	void _ExitSubClass(EnemyCharacter::State next)override {};

	void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType);




private:
	float _interval = 0.0f;// バックステップ開始までの待ち時間(アニメーションなどでタイミングがずれることがあるため)。
	Vector3 _Direction;	// 進む方向。
	bool _isBack = false;
	bool _isAddPower = false;	// 加算処理中か。
	float _nowBackPower = 0.0f;	// 後退時のバネパワー。
	float _addPower = 6.0;
	float _maxPower = 3.0f;

	float _counter = 0.0f;
};