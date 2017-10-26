#pragma once

#include "BreathObject.h"


class LaserBreath :
	public BreathObject
{
public:
	// 引数はオブジェクトの名前。
	LaserBreath(const char* name) :BreathObject(name) {

	}
	~LaserBreath() {
		INSTANCE(GameObjectManager)->AddRemoveList(_particleEmitter);
	}

	// ブレスオブジェクト初期化。
	// 引数：	ブレスを出すキャラクター。
	//			ブレス発生位置(ローカル座標、親はキャラクターのTransform)。
	//			ブレスのスピード。
	//			どの軸で回すか。
	//			回転軸(キャラクターの正面から何度回すか)。
	//			乗算カラー。
	void Init(EnemyCharacter* obj, const Vector3& emitPosLocal,float power, const Vector3& axis,float deg, Color mul = Color::white);

	void Awake()override;

	void Update()override;

	// ブレス発射開始。
	void _BreathStartSubClass()override;

	// ブレス発射終了。
	inline void _BreathEndSubClass()override {
		_particleEmitter->SetEmitFlg(false);
		_particleEmitter->AchievedCreateParticleEnd();
		_particleEmitter->SetActive(false);
	}

private:
	// 衝突判定コリジョンの更新。
	void _UpdateCollision()override;


private:
	Particle* _start = nullptr;	// 攻撃時、最初に生成されたパーティクル(ブレスの先頭)。
	Particle* _end = nullptr;	// 攻撃時、最後に生成されたパーティクル(ブレスの終端)。
	Vector3 _breathScale;

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
	Vector3 _velocity;
	float _rad;		// キャラクターの正面からどれくらい回したブレスか。
	Vector3 _axis;	// どの軸を中心に回したか。
	Color _mulColor = Color::white;
};