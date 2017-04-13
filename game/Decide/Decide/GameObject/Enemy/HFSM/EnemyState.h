#pragma once

class EnemyCharacter;

// 基底クラス。
// 雑魚とボスのステート。
class EnemyState
{
public:
	EnemyState(EnemyCharacter* Object);
	virtual ~EnemyState();

	// 切り替え時初期化。
	// ※このステートに切り替わった際に呼ばれる。
	void Entry();

	// ステート更新処理。
	// 返却値は更新処理が終了したかのフラグ。
	// ※この関数は共通処理。
	bool Update();

	// 引数は次のステート。
	// ※次のステートに移行する前に呼ばれる。
	virtual void Exit(EnemyCharacter::State next) {};

	// このステートから移行できるステートを選別する関数。
	// ※デフォルトではすべてのステートに移行できる。
	// ※継承先で上書きして実装。
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		return true;
	}


	// このステートの処理が終了しているかのフラグを返却。
	inline bool GetIsEnd() const {
		return _IsEndState;
	}

protected:
	// ローカルステート切り替え関数。
	void _ChangeLocalState(EnemyCharacter::State next);

	// ステート処理終了関数。
	inline void _EndState() {
		_IsEndState = true;
	}
private:
	// 継承先での切り替え時初期化処理。
	// ※継承先で必ず上書きして使用。
	virtual void _EntrySubClass() = 0;

	// 更新開始前初期化。
	// ※更新が始まる直前に一度だけ呼ばれる関数。
	// ※継承先で上書きして使用。
	virtual void _Start() {};

	// 継承先での更新処理。
	// ※継承先で必ず上書きして使用。
	virtual void _UpdateSubClass() = 0;

	// 現在のローカルステートが終了した際に呼ばれるコールバック関数。
	// 引数は現在のローカルステートの添え字。
	// ※処理自体は継承先に委譲。
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State now) {};
protected:
	bool _IsEndState = false;		// ステートの処理が終了したかのフラグ(trueで終了)。
	EnemyCharacter* _EnemyObject = nullptr;	// このステートを持つエネミーのポインタ。
	EnemyState* _NowLocalState = nullptr;	// 現在のローカルステート。
private:
	EnemyCharacter::State _NowLocalStateIdx;		// 現在のローカルステートの添え字。
	bool _IsFirstUpdate = true;	// ステートが切り替わってから最初の更新か。
};

