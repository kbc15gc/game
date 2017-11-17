#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

// 継承クラス。
// ボスエネミー(側近ゴースト)。
class BossGhost :
	public EnemyCharacter
{
public:
	enum class BossGhostState { BossGhostPairAttack = static_cast<int>(State::Death) + 1};

	enum PairAttackType{Combo = 0,Laser,Max};

private:
	// ボス(歩行型ドラゴン)のアニメーション番号。
	enum class AnimationBossGhost {
		Wait = 0,
		Walk,
		Attack,
		Dance,
		Damage,
		Death,
		Max
	};
public:
	BossGhost(const char* name);
	~BossGhost();

	// アニメーションイベント関連。
	void CreateCollision();
	void LaserStartSingle();
	void LaserEndSingle();
	void LaserStart();
	void LaserEnd();


	static void SelectPairAttack() {
		_selectNowPairAttack = static_cast<PairAttackType>(rand() % PairAttackType::Max);
	}

	inline void SetIsCommand(bool flg) {
		_isCommand = flg;
	}

	inline void SetPairGhost(BossGhost* ghost) {
		_pairGhost = ghost;
	}

	inline void SetIntervalStartPairAttack(float time) {
		_intervalStartPairAttack = time;
	}

	// 共同攻撃の準備完了か。
	inline bool GetIsPairAttackReady()const {
		return _isPairAttackReady;
	}

	inline EnemyAttack* GetNowPairAttack() {
		return _pairAttackArray[_selectNowPairAttack];
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

	void _BuildStateSubClass();

	// キャラクターコントローラ押し出しコンポーネントのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	// ※処理自体は継承先に委譲。
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::ENEMY);
	}

	// 継承先でアニメーション番号のテーブルを作成。
	// 引数：	アニメーション終了時間の格納用配列(この配列に終了時間を設定する、添え字はモデルに設定されているアニメーション番号)。
	// 受け取る配列内の値はデフォルトで-1となっているので、アニメーションの終了時間が1秒以上のものは設定しなくてよい。
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Oil, transform->GetPosition() + Vector3(0.0f, -1.5f, 0.0f));
	}

private:
	State _saveState;
	unique_ptr<EnemyComboAttack> _comboAttack;	// 単攻撃処理(1つのクラスがエネミーの種別なので、静的メンバでオッケーだけどエラーはいたから後回し)。
	unique_ptr<EnemyComboAttack> _laserComboAttack;
	unique_ptr<EnemySingleAttack> _singleAttack;
	unique_ptr<EnemyBreathAttack> _singleLaser;

	bool _isCommand = false;

	float _intervalStartPairAttack;
	bool _isPairAttackReady = false;	// ペア攻撃の準備完了か。
	BossGhost* _pairGhost = nullptr;	// 相方。

	static PairAttackType _selectNowPairAttack;	// 現在のペア攻撃が何か。
	vector<EnemyAttack*> _pairAttackArray;
};
