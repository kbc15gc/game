#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Enemy\BreathObject.h"
#include "GameObject\Enemy\EnemyCharacter.h"


// エネミーの攻撃処理。
// ※攻撃の種類ごとに作成。
// ※継承先で各関数を実装。
// ※このクラスのインスタンスを攻撃ステートに渡すことで各命令が呼び出される。
// アニメーションが設定されていれば自動で再生される。
class EnemyAttack {
public:
	EnemyAttack(EnemyCharacter* object) {
		_enemyObject = object;
	}
	virtual ~EnemyAttack() {

	}

	// 初期化関数。
	// 引数：	攻撃可能範囲。
	//			再生するアニメーションの種類(初期値は再生しない,モデルごとのアニメーション番号で、テーブルの番号ではない)。
	//			アニメーション補間時間(初期値は0)。
	//			アニメーションループ再生数(1でループなし、-1で無限ループ)。
	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, int animLoopNum = 1);
	virtual void Entry() = 0;	// 攻撃ステートの最初の更新前に一度だけ呼ばれる処理。
	virtual bool Update() = 0;	// 攻撃ステートの更新処理で呼び出される処理(戻り値は終了したか)。
	virtual void Exit() = 0;	// 攻撃ステート終了時に呼び出される処理。

	// この攻撃中のステート遷移を許可するか。
	// 引数：	切り替えようとしているステート。
	// ※ダメージモーションの再生許可関連だけにしようかと思ったが、ジャンプ攻撃などの実行中に落下ステートに遷移しないよう制御することを考慮した。
	inline virtual bool EnemyAttack::IsPossibleChangeState(EnemyCharacter::State next)const {
		return true;
	}


	inline const int GetAnimationType()const {
		return _animType;
	}
	inline const float GetInterpolate()const {
		return _interpolate;
	}
	inline const int GetAnimationLoopNum() const {
		return _animLoopNum;
	}
	inline void SetIsPlaying(bool flg) {
		_isPlaying = flg;
	}
	inline float GetAttackRange()const {
		return _AttackRange;
	}
protected:
	int _animType = -1;	// 再生するアニメーションの種類(初期値は再生しない,モデルごとのアニメーション番号で、テーブルの番号ではない)。
	float _interpolate = 0.0f;	// アニメーション補間時間(初期値は0)。
	int _animLoopNum = 1;	// アニメーションループ再生数(1でループなし、-1で無限ループ)。
	bool _isPlaying = false;	// アニメーション再生中かのフラグ(更新処理時に攻撃ステートから設定される)。
	float _AttackRange = 0.0f;	// 攻撃可能範囲。
	EnemyCharacter* _enemyObject = nullptr;
};

// ※単攻撃(攻撃モーション一回分攻撃)。
class EnemySingleAttack :public EnemyAttack {
public:
	EnemySingleAttack(EnemyCharacter* object);
	void Entry()override;
	bool Update()override;

	void Exit()override {};

private:
	GameObject* _player = nullptr;
};

class EnemyBreathAttack : public EnemyAttack {
public:
	// 引数：	このブレス攻撃を行うエネミー。
	EnemyBreathAttack(EnemyCharacter* object);

	~EnemyBreathAttack() {
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		if (_isBreathStart) {
			// ブレスが発射されていればブレスオブジェクトは自発的に消滅する。
			BreathEnd();
			_isBreathStart = false;
		}
	};

	// ブレス開始。
	// テンプレート引数：	使用するブレス。
	// 引数：	ブレスを発射するキャラクター。
	//			ブレスの発生位置。
	template<class T>
	inline void BreathStart(EnemyCharacter* obj, const Vector3& emitPosLocal) {
		_isBreathStart = true;
		_breath = INSTANCE(GameObjectManager)->AddNew<T>("breath", 8);
		_breath->SetActive(true);
		_breath->Init(obj, emitPosLocal);
		_breath->BreathStart();
	}

	// ブレス終了。
	inline void BreathEnd() {
		if (_breath) {
			_breath->BreathEnd();
			_breath = nullptr;
		}
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// ブレスオブジェクト(ブレス発射処理が終わった後もブレスの挙動を管理できるようにするためにクラス化した)。
	bool _isBreathStart = false;
};