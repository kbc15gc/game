#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"

class LastBossMagic;
class SordShock;

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
		Wait = 0,
		SordAttack,
		Magic,
		Damage,
		Move,
		Max,
	};

public:
	LastBoss(const char* name);
	~LastBoss();


	void SordAttackEvent();
	void SordAttackEvent2();
	void MagicAttackStart1();
	void MagicAttackShot1();
	void MagicAttackStart2();
	void MagicAttackShot2();
	void MagicAttackStart3();
	void MagicAttackShot3();
	void BuffDebuffEvent();

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

	inline void SetSaveState(LastBossState state) {
		_saveState = state;
	}

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
	// 引数：	アニメーション終了時間の格納用配列(この配列に終了時間を設定する、添え字はモデルに設定されているアニメーション番号)。
	// 受け取る配列内の値はデフォルトで-1となっているので、アニメーションの終了時間が1秒以上のものは設定しなくてよい。
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	// 効果音のテーブル作成関数。
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
	}


private:
	LastBossState _saveState;
	unique_ptr<EnemySingleAttack> _sordAttack;	// 単攻撃処理。
	unique_ptr<EnemySingleAttack> _buffAttack;
	unique_ptr<EnemySingleAttack> _debuffAttack;
	unique_ptr<EnemyBreathAttack> _magicAttack;

	LastBossMagic* _magicFire1 = nullptr;
	LastBossMagic* _magicFire2 = nullptr;
	LastBossMagic* _magicFire3 = nullptr;

	SordShock* _sordAttackShot0 = nullptr;
	SordShock* _sordAttackShot1 = nullptr;
	SordShock* _sordAttackShot2 = nullptr;
};