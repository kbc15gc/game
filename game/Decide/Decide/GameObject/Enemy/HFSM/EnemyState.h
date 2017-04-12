#pragma once

class EnemyCharacter;

// 基底クラス。
// 雑魚とボスのステート。
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	virtual void Entry();

	// 返却値は更新処理が終了したかのフラグ。
	virtual bool Update();

	// 引数は次のステート。
	virtual void Exit(EnemyCharacter::State next);

protected:
	// ローカルステート切り替え関数。
	void _ChangeLocalState(EnemyCharacter::State next);

private:
	// 現在のローカルステートが終了した際に呼ばれるコールバック関数。
	// 引数は現在のローカルステートの添え字。
	// ※処理自体は継承先に委譲。
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
private:
	EnemyCharacter* _EnemyObject = nullptr;	// このステートを持つエネミーのポインタ。

	EnemyState* _NowLocalState = nullptr;	// 現在のローカルステート。
	EnemyCharacter::State _NowLocalStateIdx;		// 現在のローカルステートの添え字。

	bool _IsEndState = false;		// ステートの処理が終了したかのフラグ(trueで終了)。
};

