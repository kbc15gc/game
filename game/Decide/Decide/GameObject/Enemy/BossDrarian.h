#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"


// 継承クラス。
// ボスエネミー(歩行型ドラゴン)。
class BossDrarian :
	public EnemyCharacter
{
private:
	// ボス(歩行型ドラゴン)のアニメーション番号。
	enum class AnimationDrarian {
		Wait = 0,
		Walk,
		Dash,
		Attack,
		Barking,
		TailAttackRight,
		Breath,
		Damage,
		Death,
		Max
	};
public:
	BossDrarian(const char* name);
	~BossDrarian();

	void CreateAttackCollision_Kamituki();

protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* AttackSelect()override;

	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

	// キャラクターコントローラーのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	void _ConfigCharacterController()override;

	// 継承先でアニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	void _BuildAnimation()override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	// 効果音のテーブル作成関数。
	void _BuildSoundTable()override;
private:
	State _saveState;
	EnemySingleAttack _singleAttack;	// 単攻撃処理(1つのクラスがエネミーの種別なので、静的メンバでオッケーだけどエラーはいたから後回し)。
	EnemySingleAttack _tailAttack;
};


class EnemyBreathAttack : public EnemyAttack {
public:
	EnemyBreathAttack() {
		_player = INSTANCE(GameObjectManager)->FindObject("Player");
		_particleEmitter = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);
	}

	// パーティクルのパラメーターを設定。
	void ConfigParticleParameter(ParticleParameter param) {
		_particleEmitter->Init(param);
	}

	void Start()override {
		_enemyObject->LookAtObject(*_player);
	};
	bool Update()override;

	void Exit()override {};

private:
	GameObject* _player = nullptr;
	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _particleParam;
};