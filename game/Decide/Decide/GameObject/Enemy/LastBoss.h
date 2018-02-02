#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "Scene\GameScene.h"

class LastBossMagic;
class SordShock;

// 継承クラス。
// ボスエネミー(ラスボス)。
class LastBoss :
	public EnemyCharacter
{
public:
	enum class LastBossState { LastBossThrone = static_cast<int>(State::Death) + 1, LastBossMagician, LastBossHistory, LastBossDown };

	enum class LastBossSoundIndex { Fire2 = static_cast<int>(EnemyCharacter::SoundIndex::Max),Fire3,Shot1, Shot2,Shot3,Danmatuma,Cyouhatu,MagicianStart,Warau,ChargeSord,WarpSord, ChargeFire,Damage/*,Battle1,Battle2*/};

	// エネミー(ラスボス)のアニメーション番号。
	enum class AnimationLastBoss {
		Wait = 0,
		SordAttack,
		Magic,
		Damage,
		Move,
		Death,
		WaitThrone,	// 玉座待機。
		MoveThrone,	// 玉座移動。
		MagicThrone,	// 玉座魔法。
		ThroneEnd,	// 玉座削除。
		HistoryStart,	// 大魔法杖出し。
		//HistoryCharge,	// 大魔法詠唱。

		Max,
	};

private:
public:
	LastBoss(const char* name);
	~LastBoss();


	void ChargeSordSpeed();
	void SordAttackEvent();
	void SordAttackEvent2();
	void SordAttackEvent3();
	void SordAttackEvent4();
	void FastSord();
	void FastSord2();
	void MagicAttackSpeed1();
	void MagicAttackStart1();
	void MagicAttackShot1();
	void MagicAttackStart2();
	void MagicAttackShot2();
	void MagicAttackStart3();
	void MagicAttackShot3();

	void HighMagicAttackStart1();
	void HighMagicAttackShot1();
	void HighMagicAttackStart2();
	void HighMagicAttackShot2();
	void HighMagicAttackStart3();
	void HighMagicAttackShot3();
	void HighMagicAttackStart4();
	void HighMagicAttackShot4();
	void HighMagicAttackStart5();
	void HighMagicAttackShot5();
	void HighMagicAttackStart6();
	void HighMagicAttackShot6();

	void Danmatuma();

	void BuffEvent();
	void DebuffEvent();
	void EntourageCommand();
	void EncourageBuff();
	void Special();

	void PlayVoiceCyouhatu();
	void PlayVoiceMagicStart();
	void PlayVoiceWarau();
	void PlayVoiceChargeSord();
	void PlayVoiceWarpSord();
	void PlayVoiceChargeFire();
	void PlayVoiceDamage();

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

	inline void SetSaveState(LastBossState state) {
		_saveState = state;
	}

	inline EnemySingleAttack* GetSpecialAttack()const {
		return _specialAttack.get();
	}

	inline EnemySingleAttack*GetEncourageBuffAttack()const {
		return _encourageBuffAttack.get();
	}

	// バトル終了。
	void BattleEnd() {
		if (_isStartBattle) {
			static_cast<GameScene*>(INSTANCE(SceneManager)->GetNowScene())->ResetBGMIndex();
			_ChangeState(_initState);
			_isStartBattle = false;
		}
	}

	void SetActive(const bool act, const bool children = false)override;

	void SetIsStartBattle(bool flg) {
		_isStartBattle = flg;
	}
protected:
	void _EndNowStateCallback(State EndStateType)override;

	// ステートが切り替わったときに呼ばれるコールバック。
	// 引数：	切り替わる前のステートタイプ。
	//			切り替わった後のステートタイプ。
	// ※処理自体は継承先で実装。
	void _ChangeStateCallback(State prev, State next)override;

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

	inline void _DropSubClass()override {
	}


private:
	LastBossState _saveState;
	unique_ptr<EnemySingleAttack> _sordAttack;	// 単攻撃処理。
	unique_ptr<EnemySingleAttack> _sordAttack2;	// 単攻撃処理。
	unique_ptr<EnemyWarpAttack> _warpAttack;
	unique_ptr<EnemyBreathAttack> _magicAttack;
	unique_ptr<EnemyBreathAttack> _highMagicAttack;

	unique_ptr<EnemySingleAttack> _buffAttack;
	unique_ptr<EnemySingleAttack> _debuffAttack;
	unique_ptr<EnemySingleAttack> _commandAttack;
	unique_ptr<EnemySingleAttack> _encourageBuffAttack;
	unique_ptr<EnemySingleAttack> _specialAttack;


	LastBossMagic* _magicFire1 = nullptr;
	LastBossMagic* _magicFire2 = nullptr;
	LastBossMagic* _magicFire3 = nullptr;

	LastBossMagic* _magicBigFire1 = nullptr;
	LastBossMagic* _magicBigFire2 = nullptr;
	LastBossMagic* _magicBigFire3 = nullptr;
	LastBossMagic* _magicBigFire4 = nullptr;
	LastBossMagic* _magicBigFire5 = nullptr;
	LastBossMagic* _magicBigFire6 = nullptr;

	SordShock* _sordAttackShot0 = nullptr;
	SordShock* _sordAttackShot1 = nullptr;
	SordShock* _sordAttackShot2 = nullptr;

	bool _isStartBattle = false;

	GameScene* _scene = nullptr;
};