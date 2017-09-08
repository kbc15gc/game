#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

class EnemyBreathAttack;

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

	// アニメーションイベント関連。
	void AnimationEvent_Kamituki();
	void CreateAttackCollision_TailAttack1();
	void CreateAttackCollision_TailAttack2();
	void CreateAttackCollision_TailAttack3();
	void CreateAttackCollision_TailAttack4();
	void AnimationEvent_BreathStart();
	void AnimationEvent_BreathEnd();


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

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Oil, transform->GetPosition());
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// 単攻撃処理(1つのクラスがエネミーの種別なので、静的メンバでオッケーだけどエラーはいたから後回し)。
	unique_ptr<EnemySingleAttack> _tailAttack;
	unique_ptr<EnemyBreathAttack> _breathAttack;
};


class EnemyBreathAttack : public EnemyAttack {
private:
	EnemyBreathAttack(EnemyCharacter* object):EnemyAttack(object) {};
public:
	// 引数：	このブレス攻撃を行うエネミー。
	//			生成するパーティクルのパラメータ。
	//			生成位置(ローカル座標、親はブレスを発射するエネミー)。	
	EnemyBreathAttack(EnemyCharacter* object, ParticleParameter& param, const Vector3& emitPos);

	~EnemyBreathAttack() {
		ReleaceEmitterAll();
	}

	void Entry()override {
		_breath = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
		_breath->Init(_enemyObject);
		_enemyObject->LookAtObject(*_player);
		// パーティクルの飛ぶ方向をエネミーの向きに再設定。
		Vector3 initVelocity = _enemyObject->transform->GetForward();
		initVelocity.Normalize();
		_initParticleParam.initVelocity = initVelocity * _particleEmitter[0]->GetInitVelocity().Length();
		_particleEmitter[0]->SetParam(_initParticleParam);
	}

	bool Update()override;

	void Exit()override {
		BreathEnd();
		_breath = nullptr;	// ブレスオブジェクトは自発的に消滅するため放置。
	};

	// パーティクルエミッター追加。
	// 生成するパーティクルのパラメータ。
	// 生成位置。
	// 親(デフォルトはnull)。
	// ※この関数を呼んだだけではエミットは開始されない。
	// ※エミットする際はエミッターのSetEmitFlg関数を呼ぶ。
	void AddParticleEmitter(const ParticleParameter& param, const Vector3& emitPos, Transform* parent = nullptr) {
		ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);
		p->transform->SetParent(parent);
		p->transform->SetLocalPosition(emitPos);
		p->Init(param);
		p->SetEmitFlg(false);
		_particleEmitter.push_back(p);
	}

	// ブレス開始。
	inline void BreathStart() {
		unique_ptr<vector<Particle*>> list;
		list.reset(new vector<Particle*>);
		_particleEmitter[0]->AchievedCreateParticleStart(list.get());
		_breath->SetParticleList(move(list));
		_particleEmitter[0]->SetEmitFlg(true);
		_breath->BreathStart();
	}

	// ブレス終了。
	inline void BreathEnd() {
		_particleEmitter[0]->SetEmitFlg(false);
		_particleEmitter[0]->AchievedCreateParticleEnd();
	}

	void ReleaceEmitterAll() {
		for (auto emitter : _particleEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(emitter);
		}
		_particleEmitter.clear();
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// ブレスオブジェクト(ブレス発射処理が終わった後もブレスの挙動を管理できるようにするためにクラス化した)。
	vector<ParticleEmitter*> _particleEmitter;
	ParticleParameter _initParticleParam;
};