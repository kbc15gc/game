#pragma once

#include "GameObject\Enemy\EnemyCharacter.h"
#include "GameObject\Enemy\EnemyManager.h"

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

	virtual void LateUpdate() {};

	// 引数は次のステート。
	// ※次のステートに移行する際に呼ばれる。
	void Exit(EnemyCharacter::State next);

	// このステートから移行できるステートを選別する関数。
	// ※デフォルトではすべてのステートに移行できる。
	// ※継承先で上書きして実装。
	inline virtual bool IsPossibleChangeState(EnemyCharacter::State next) {
		return true;
	}

	// このステート中にダメージを与えられるか。
	// ※デフォルトはダメージを与えられる。
	// ※継承先で上書きして実装。
	inline virtual bool IsPossibleDamage() {
		return true;
	}

	// ステート処理終了要請関数。
	// ※外部から強制的にステート処理を終了させたい場合はこちらを呼ぶ。
	inline void _EndStateRequest() {
		_EndState();
	}


	// このステートの処理が終了しているかのフラグを返却。
	inline bool GetIsEnd() const {
		return _IsEndState;
	}

	// 指定した値で使用するアニメーションや補間時間を書き換える。
	inline void CustamParameter(EnemyCharacter::AnimationType type, float interpolate,int loopNum,int eventNo,float speed) {
		_playAnimation = type;
		_interpolate = interpolate;
		_loopNum = loopNum;
		_eventNo = eventNo;
		_playSpeed = speed;
	}

	EnemyCharacter::AnimationType GetPlayAnimationType()const {
		return _playAnimation;
	}

	float GetInterpolate()const {
		return _interpolate;
	}

	int GetEventNo()const {
		return _eventNo;
	}

	int GetLoopNum()const {
		return _loopNum;
	}

	float GetPlaySpeed()const {
		return _playSpeed;
	}

	virtual void SetActive(bool flg) {
		_isActive = flg;
	};

protected:
	// ローカルステート切り替え関数。
	void _ChangeLocalState(EnemyCharacter::State next);

	// ステート処理終了関数。
	inline void _EndState() {
		_IsEndState = true;
	}

	void _PlayAnimation() {
		_saveSpeed = _EnemyObject->GetAnimationSpeed();
		_EnemyObject->SetAnimationSpeed(_playSpeed);
		_EnemyObject->PlayAnimation(_playAnimation, _interpolate, _loopNum, _eventNo);
	}

private:
	// 継承先での切り替え時初期化処理。
	// ※継承先で必ず上書きして使用。
	virtual void _EntrySubClass() = 0;

	void Start() {
		_PlayAnimation();
		_StartSubClass();
	}

	// 更新開始前初期化。
	// ※更新が始まる直前に一度だけ呼ばれる関数。
	// ※継承先で上書きして使用。
	virtual void _StartSubClass() {};

	// 継承先での更新処理。
	// ※継承先で必ず上書きして使用。
	virtual void _UpdateSubClass() = 0;

	// 引数は次のステート。
	// ※次のステートに移行する際に呼ばれる。
	// ※継承先で実装。
	virtual void _ExitSubClass(EnemyCharacter::State next) = 0;

	// 現在のローカルステートが終了した際に呼ばれるコールバック関数。
	// 引数は現在のローカルステートの添え字。
	// ※この関数は各ステートが自発的に終了した場合にのみ呼び出される。
	// ※処理自体は継承先に委譲。
	virtual void _EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {};


protected:

	EnemyCharacter::AnimationType _playAnimation = EnemyCharacter::AnimationType::None;
	float _interpolate = 0.2f;
	int _loopNum = 1;
	int _eventNo = 0;
	float _playSpeed = 1.0f;
	float _saveSpeed;

	bool _IsEndState = false;		// ステートの処理が終了したかのフラグ(trueで終了)。
	EnemyCharacter* _EnemyObject = nullptr;	// このステートを持つエネミーのポインタ。
	EnemyState* _NowLocalState = nullptr;	// 現在のローカルステート。
	EnemyCharacter::State _NowLocalStateIdx;		// 現在のローカルステートの添え字。
private:

	bool _IsFirstUpdate = true;	// ステートが切り替わってから最初の更新か。

	bool _isActive = true;
};

