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
	void Init(EnemyCharacter* obj, const Vector3& emitPosLocal)override;

	void Awake()override;

	void Update()override;

	// ブレス発射開始。
	void BreathStart();

	// ブレス発射終了。
	inline void BreathEnd()override {
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
};