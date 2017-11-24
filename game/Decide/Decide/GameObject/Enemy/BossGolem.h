#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"
#include "GameObject\History\HistoryManager.h"

// 継承クラス。
// ボスエネミー(中ボスゴーレム)。
class BossGolem :
	public EnemyCharacter
{
private:
	// ボス(中ボスゴーレム)のアニメーション番号。
	enum class AnimationBossGolem
	{
		Idle = 0,
		IdleAction,
		SleepS,
		SleepL,
		SleepE,
		Damage,
		Hit,
		Die,
		Walk,
		Hit2,
		Rage,
		Jump,
		Fly,
		Land
	};

public:
	BossGolem(const char* name);
	~BossGolem();

	// アニメーションイベント関連。
	void AnimationEvent_Kobushi();
	void AnimationEvent_Zutuki();
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

	inline void _DropSubClass()override {
		if (!INSTANCE(HistoryManager)->IsSetChip(ChipID::Copper))
		{
			Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
			chip->SetDropChipID(ChipID::Copper, transform->GetPosition() + Vector3(0.0f, -1.5f, 0.0f));

		}
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// 単攻撃処理(1つのクラスがエネミーの種別なので、静的メンバでオッケーだけどエラーはいたから後回し)。
	unique_ptr<EnemySingleAttack> _singleAttackSecondPattern;
};
