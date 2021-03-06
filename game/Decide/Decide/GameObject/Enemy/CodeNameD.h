#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"
#include "GameObject\History\HistoryManager.h"

// 継承クラス。
// ボスエネミー(ヒューマン型）
class BossD :
	public EnemyCharacter
{
private:
	// ボス(中ボス)のアニメーション番号。
	enum class AnimationBossD
	{
		Idle = 0,
		Walk,
		Run,
		DrawingaSword,
		Kakusei,
		Attack,
		Kick,
		Kiriage,
		Attack360,
		Damage
	};

public:
	BossD(const char* name);
	~BossD();

	// アニメーションイベント関連。
	void AnimationEvent_Attack();
	void AnimationEvent_Kick();
	void AnimationEvent_Kiriage();
	void AnimationEvent_KiriageTame();
	void AnimationEvent_KiriageTame2();
	void AnimationEventPlay360();	// 回転攻撃にシフト。
	void AnimationEvent_360Attack();
	void AniamtionEvent_360AttackCombo();
	void AniamtionEvent_KiriageCombo();

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

	// 継承先でアニメーション番号のテーブルを作成。
	// 引数：	アニメーション終了時間の格納用配列(この配列に終了時間を設定する、添え字はモデルに設定されているアニメーション番号)。
	// 受け取る配列内の値はデフォルトで-1となっているので、アニメーションの終了時間が1秒以上のものは設定しなくてよい。
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	// 効果音のテーブル作成関数。
	

	inline void _DropSubClass()override {
		if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Medicine))
		{
			Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
			chip->SetDropChipID(ChipID::Medicine, transform->GetPosition() + Vector3(0.0f, 1.0f, 0.0f));
			chip->SetGetTime(ChipTimer);
		}
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// 単攻撃処理(1つのクラスがエネミーの種別なので、静的メンバでオッケーだけどエラーはいたから後回し)。
	unique_ptr<EnemySingleAttack> _kick;
	unique_ptr<EnemySingleAttack> _kiriage;
	unique_ptr<EnemySingleAttack> _360Attack;
	unique_ptr<EnemySingleAttack> _combo;
	unique_ptr<EnemySingleAttack> _3combo;
};
