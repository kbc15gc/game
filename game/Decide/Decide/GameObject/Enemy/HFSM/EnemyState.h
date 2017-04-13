#pragma once

class EnemyCharacter;

// 基底クラス。
// 雑魚とボスのステート。
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	// 初期化。
	// ※このステートに切り替わった際に呼ばれる。
	virtual void Entry();

	// ステート更新処理。
	// 返却値は更新処理が終了したかのフラグ。
	// ※この関数は共通処理。
	bool Update();

	// 引数は次のステート。
	// ※次のステートに移行する前に呼ばれる。
	virtual void Exit(EnemyCharacter::State next);

protected:
	// ローカルステート切り替え関数。
	void _ChangeLocalState(EnemyCharacter::State next);

private:
	// 継承先での更新処理。
	// ※継承先で上書きして使用。
	virtual void _UpdateSubClass() = 0;

	// 現在のローカルステートが終了した際に呼ばれるコールバック関数。
	// 引数は現在のローカルステートの添え字。
	// ※処理自体は継承先に委譲。
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
protected:
	bool _IsEndState = false;		// ステートの処理が終了したかのフラグ(trueで終了)。
	EnemyCharacter* _EnemyObject = nullptr;	// このステートを持つエネミーのポインタ。
private:

	EnemyState* _NowLocalState = nullptr;	// 現在のローカルステート。
	EnemyCharacter::State _NowLocalStateIdx;		// 現在のローカルステートの添え字。
};

