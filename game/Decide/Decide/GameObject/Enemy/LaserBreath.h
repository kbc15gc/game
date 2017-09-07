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

	}

	void Start()override;

	void Update()override;

	// ブレス発射開始。
	void BreathStart();

	// ブレス発射終了。
	inline void BreathEnd()override {
	}

private:
	// 衝突判定コリジョンの更新。
	void _UpdateCollision()override;


private:
	Particle* _start = nullptr;	// 攻撃時、最初に生成されたパーティクル(ブレスの先頭)。
	Particle* _end = nullptr;	// 攻撃時、最後に生成されたパーティクル(ブレスの終端)。
	Vector3 _breathScale;
};