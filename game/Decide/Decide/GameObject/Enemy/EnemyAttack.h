#pragma once

#include "GameObject\Player\Player.h"
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
	//			アニメーション再生速度(1.0より大きい値で再生速度が速くなり、小さい値で遅くなる、デフォルトは1.0)。
	//			アニメーションループ再生数(1でループなし、-1で無限ループ)。
	//			使用するアニメーションイベント番号(指定した番号のイベントリストに登録されたイベントが呼び出される、デフォルトは0)。
	virtual void Init(float attackRange, int animType = -1, float interpolate = 0.0f,float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0);
	virtual void Entry();	// 攻撃ステートの最初の更新前に一度だけ呼ばれる処理。
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
	inline int GetPlayEventNo()const {
		return _playEventNo;
	}
protected:
	int _animType = -1;	// 再生するアニメーションの種類(初期値は再生しない,モデルごとのアニメーション番号で、テーブルの番号ではない)。
	float _interpolate = 0.0f;	// アニメーション補間時間(初期値は0)。
	float _playSpeed = 1.0f;
	int _animLoopNum = 1;	// アニメーションループ再生数(1でループなし、-1で無限ループ)。
	int _playEventNo = 0;	// 使用するアニメーションイベント番号。
	bool _isPlaying = false;	// アニメーション再生中かのフラグ(更新処理時に攻撃ステートから設定される)。
	float _AttackRange = 0.0f;	// 攻撃可能範囲。
	EnemyCharacter* _enemyObject = nullptr;

public:
	bool warptest = false;

};

// ※単攻撃(攻撃モーション一回分攻撃)。
class EnemySingleAttack :public EnemyAttack {
public:
	EnemySingleAttack(EnemyCharacter* object);
	void Entry()override;

	// 戻り値：	攻撃が終了したか。
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
		if (_breath) {
			if (_breath->GetIsStart()) {
				// ブレス発射中に攻撃がキャンセルされた。
				BreathEnd();
			}
		}
	};

	// 一回の攻撃に使用するブレスオブジェクト追加。
	// ブレス攻撃のたびに毎回1から追加する。
	// 引数：	使用するブレスオブジェクト。
	inline void BreathStart(BreathObject* obj) {
		if (warptest) {
			OutputDebugString("warptest\n");
		}
		_breath = obj;
		_breath->SetActive(true);
		_breath->BreathStart();
	}

	// ブレス終了。
	inline void BreathEnd() {
		if (_breath) {
			if (warptest) {
				OutputDebugString("warptest\n");
				test = _breath;
				char testc[256];
				sprintf(testc, "test = %p\n", test);
				OutputDebugString(testc);
			}
			_breath->BreathEnd();
			_breath = nullptr;
		}
	}

	inline BreathObject* GetBreathObject() {
		return _breath;
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// ブレスオブジェクト(ブレス発射処理が終わった後もブレスの挙動を管理できるようにするためにクラス化した)。
public:
	BreathObject* test = nullptr;
};

class EnemyWarpAttack : public EnemyAttack {
private:
	enum WarpState { Through, Materialization };
public:
	EnemyWarpAttack(EnemyCharacter* object) :EnemyAttack(object) {
		_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	}
	~EnemyWarpAttack() {
	};

	// 連続攻撃の初期化関数。
	// 引数：	攻撃可能範囲(ここでは攻撃開始可能範囲)。
	//			攻撃一回を定義したクラスのポインタ(Init関数を呼んだ後のものを渡す)。
	void Init(float attackRange, EnemyAttack* attack) {
		_AttackRange = attackRange;
		_attack.reset(attack);
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		_enemyObject->SetAlpha(1.0f);
		_attack->Exit();
		_isAttackEnd = false;
	}

private:
	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0)override {
		//EnemyAttack::Init(attackRange, animType, interpolate, playSpeed, animLoopNum, playEventNo);
	}

private:
	unique_ptr<EnemyAttack> _attack;
	WarpState _nowWarpState;
	Player* _player = nullptr;
	bool _isWarpEnd = false;	// 実体化完了か。
	bool _isAttackEnd = false;
	const float _chaceTime = 0.45f;
	float _counter = 0.0f;
};

class EnemyComboAttack:public EnemyAttack{
public:
	EnemyComboAttack(EnemyCharacter* object) :EnemyAttack(object) {
		_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	}
	~EnemyComboAttack() {
	};

	// 連続攻撃の初期化関数。
	// 引数：	攻撃可能範囲(ここでは攻撃開始可能範囲)。
	//			攻撃一回を定義したクラスのポインタ(Init関数を呼んだ後のものを渡す)。
	void Init(float attackRange,EnemyAttack* oneAttack) {
		_AttackRange = attackRange;
		_oneCombo.reset(oneAttack);
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		_enemyObject->SetAlpha(1.0f);
		_oneCombo->Exit();
	}


	inline EnemyAttack* GetOneAttack() {
		return _oneCombo.get();
	}
private:

	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0)override {
		//EnemyAttack::Init(attackRange, animType, interpolate, playSpeed, animLoopNum, playEventNo);
	}


private:
	Player* _player = nullptr;
	unique_ptr<EnemyAttack> _oneCombo;
	int _attackNum = 4;
	int _comboCount;
	bool _isStartAttack = false;
	//const float _chaceTime = 0.4f;
};