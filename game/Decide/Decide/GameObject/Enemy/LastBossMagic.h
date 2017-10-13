#pragma once

#include "GameObject\Enemy\BreathObject.h"

class LastBossMagic :
	public BreathObject
{
public:
	// 引数はオブジェクトの名前。
	LastBossMagic(const char* name) :BreathObject(name) {

	}
	~LastBossMagic() {
		INSTANCE(GameObjectManager)->AddRemoveList(_particleEmitter);
	}

	// ブレスオブジェクト初期化。
	// 引数：	ブレスを出すキャラクター。
	//			ブレス発生位置(ローカル座標、親はキャラクターのTransform)。
	void Init(EnemyCharacter* obj, const Vector3& emitPosLocal)override;

	void Awake()override;

	void Update()override;

	// ブレス発射開始。
	void BreathStart()override;

	// ブレス発射終了。
	inline void BreathEnd()override {
	}

private:

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
	Vector3 _initEmitPos = Vector3::zero;
	Vector3 _direction = Vector3::zero;
	float _timeCounter;
	float _interval = 3.0f;
};