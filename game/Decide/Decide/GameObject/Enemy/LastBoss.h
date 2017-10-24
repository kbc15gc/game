#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"


// 継承クラス。
// ボスエネミー(ラスボス)。
class LastBoss :
	public EnemyCharacter
{
public:
	enum class LastBossState { LastBossThrone = static_cast<int>(State::Death) + 1, LastBossMagician, LastBossHistory, LastBossDown };

private:
	// エネミー(ラスボス)のアニメーション番号。
	enum class AnimationLastBoss {
		Move = 0,
		SordAttack,
		Magic,
		Max,
	};

public:
	LastBoss(const char* name);
	~LastBoss();

	void SordAttackEvent();
	void SordAttackEvent2();
	void MagicAttackStart1();
	void MagicAttackEnd1();
	void MagicAttackStart2();
	void MagicAttackEnd2();
	void MagicAttackStart3();
	void MagicAttackEnd3();

protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* _AttackSelectSubClass()override;

	// コリジョン定義関数。
	// コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

	// キャラクターコントローラーのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	void _ConfigCharacterController()override;

	// キャラクターコントローラ押し出しコンポーネント用の剛体作成関数。
	// コリジョン属性や形状などを設定し、作成する。
	void _CreateExtrudeCollision()override;

	// キャラクターコントローラ押し出しコンポーネントのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	// ※処理自体は継承先に委譲。
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::ENEMY);
	}

	// 絶対に他のクラスでも使わないステートはこっちに登録。
	void _BuildStateSubClass()override;

	// アニメーション番号のテーブルを作成。
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	void _BuildAnimation()override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	// 効果音のテーブル作成関数。
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _sordAttack;	// 単攻撃処理。
	unique_ptr<EnemyBreathAttack> _magicAttack;

	LaserBreath* _sordAttackLaser = nullptr;
};