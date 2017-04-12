#include "EnemyState.h"

EnemyState::EnemyState(EnemyCharacter* Object)
{
	_EnemyObject = Object;
}


EnemyState::~EnemyState()
{
}

void EnemyState::Entry() {
	_IsEndState = true;
}

bool EnemyState::Update() {
	if (_NowLocalState) {
		// 現在のローカルステートが設定されている。
		if (_NowLocalState->Update()) {
			// ローカルステートの更新処理が終了した。

			// コールバック呼び出し。
			// ※関数の内部処理は継承先で実装。
			_EndNowLocalState_CallBack(_NowLocalStateIdx);
		}
	}
	return _IsEndState;
}

void EnemyState::Exit(EnemyCharacter::State next) {

}

void EnemyState::_ChangeLocalState(EnemyCharacter::State next) {
	// エネミーの持つステート配列を参照で受け取る。
	const vector<unique_ptr<EnemyState>>& enemyState = _EnemyObject->GetMyState();
	
	if (static_cast<int>(next) >= enemyState.size() || static_cast<int>(next) < 0) {
		// 渡された数字が配列の容量を超えている。
		abort();
	}

	if (_NowLocalState) {
		// 現在のステートがnullでない。
		// 現在のステートを後片付け。
		_NowLocalState->Exit(next);
	}

	// 新しいステートに切り替え。
	_NowLocalState = enemyState[static_cast<int>(next)].get();
	_NowLocalState->Entry();

	// 現在のステートの添え字を保存。
	_NowLocalStateIdx = next;
}