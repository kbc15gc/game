#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyState.h"


EnemyState::EnemyState(EnemyCharacter* Object)
{
	_EnemyObject = Object;
}


EnemyState::~EnemyState()
{
}

void EnemyState::Entry() {
	_NowLocalState = nullptr;
	_NowLocalStateIdx = EnemyCharacter::State::None;
	_IsEndState = false;
	_IsFirstUpdate = true;
	// 継承先によって異なる処理。
	// ※純粋仮想関数。
	_EntrySubClass();
}

bool EnemyState::Update() {
	if (!_IsEndState) {
		// このステートが終了していない。

		if (_IsFirstUpdate) {
			// ステートが切り替わってから最初の更新。	
			Start();
			_IsFirstUpdate = false;
		}

		// 継承先によって異なる処理。
		// ※純粋仮想関数。
		_UpdateSubClass();

		if (_NowLocalState) {
			// 現在のローカルステートが設定されている。
			if (_NowLocalState->Update()) {
				// ローカルステートの更新処理が終了した。

				EnemyCharacter::State work = _NowLocalStateIdx;	// 終了したステートを保存。
				// とりあえずステートを切り替えて必ず終了処理を呼ぶ。
				_ChangeLocalState(EnemyCharacter::State::None);

				// コールバック呼び出し。
				// ※関数の内部処理は継承先で実装。
				_EndNowLocalState_CallBack(work);
			}
		}
	}
	return _IsEndState;
}

void EnemyState::Exit(EnemyCharacter::State next) {

	_ExitSubClass(next);

	if (_NowLocalState) {
		// ローカルステートが設定されている。
		_ChangeLocalState(EnemyCharacter::State::None);
	}
	_EnemyObject->SetAnimationSpeed(_saveSpeed);
}

void EnemyState::_ChangeLocalState(EnemyCharacter::State next) {
	// エネミーの持つステート配列を参照で受け取る。
	const vector<unique_ptr<EnemyState>>& enemyState = _EnemyObject->GetMyState();
	
	if (next != EnemyCharacter::State::None && static_cast<int>(next) >= static_cast<int>(enemyState.size())) {
		// 渡された数字が配列の容量を超えている。
		abort();
	}

	if (_NowLocalState) {
		// 現在のステートがnullでない。
		// 現在のステートを後片付け。
		_NowLocalState->Exit(next);
	}

	if (next == EnemyCharacter::State::None) {
		// 次のステートが指定されていない。
		_NowLocalState = nullptr;
	}
	else {
		// 新しいステートに切り替え。
		_NowLocalState = enemyState[static_cast<int>(next)].get();
		_NowLocalState->Entry();
	}

	// 現在のステートの添え字を保存。
	_NowLocalStateIdx = next;
}