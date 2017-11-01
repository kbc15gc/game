#pragma once

#include "GameObject\Enemy\BreathObject.h"

// ボスの剣攻撃の衝撃波。
class SordShock :
	public BreathObject
{
public:
	// 引数はオブジェクトの名前。
	SordShock(const char* name) :BreathObject(name) {

	}
	~SordShock() {
		if (_tirotiroEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(_tirotiroEmitter);
		}
		if (_shockParticleEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(_shockParticleEmitter);
		}
	}

	// ブレスオブジェクト初期化。
	// 引数：	ブレスを出すキャラクター。
	//			ブレス発生位置(ローカル座標、親はキャラクターのTransform)。
	//			衝撃波の進行速度。
	//			衝撃波の飛距離。
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal, const Vector3& speed,const float range);

	void Awake()override;

	void Update()override;

	// ブレス発射開始。
	void _BreathStartSubClass()override;

	// ブレス発射終了。
	inline void _BreathEndSubClass()override {
	}

private:

	ParticleEmitter* _shockParticleEmitter = nullptr;
	ParticleParameter _initShockParticleParam;
	Vector3 _emitPos = Vector3::zero;
	Vector3 _speed = Vector3::zero;
	float _range = 0.0f;	// 衝撃波の飛距離。


	ParticleEmitter* _tirotiroEmitter = nullptr;
	ParticleParameter _initTirotiroParticleParam;

	bool _isShot = false;
	bool _isBomb = false;
	bool _isAutoRange = false;

	// ちろちろが残る時間。
	float _timeCounter;
	float _interval = 15.0f;
};