#pragma once

#include "fbEngine\fbstdafx.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\Particle.h"

class EnemyCharacter;

// ブレスオブジェクト。
// (ブレス発射処理が終わった後もブレスの挙動を管理できるようにするためにクラス化した)。
class BreathObject :
	public GameObject
{
public:
	// 引数はオブジェクトの名前。
	BreathObject(const char* name) :GameObject(name) {

	}
	~BreathObject() {
	}

	void OnDestroy()override {
		if (_isStart) {
			BreathEnd();
		}
		ReleaseCollisionAll();
		ReleaceParticleAll();
	}

	// ブレス発射開始。
	inline void BreathStart() {
		_isStart = true;
		_BreathStartSubClass();
	}

	// ブレス発射終了。
	inline void BreathEnd() {
		_isStart = false;
		_BreathEndSubClass();
	}

	void ReleaseCollisionAll() {
		for (auto attack : _attack) {
			INSTANCE(GameObjectManager)->AddRemoveList(attack);
		}
		_attack.clear();
	}

	void ReleaceParticleAll() {
		if (_particleList) {
			for (auto particle : *_particleList) {
				particle->SetIsDead(true);
			}
			_particleList->clear();
		}
	}


	inline bool GetIsStart()const {
		return _isStart;
	}

protected:
	// 初期化。
	void Init(EnemyCharacter* enemy) {
		_enemyObject = enemy;
	}

	virtual void _BreathStartSubClass() = 0;

	virtual void _BreathEndSubClass() = 0;

private:
	// 衝突判定コリジョンの更新。
	virtual void _UpdateCollision() {};

protected:
	unique_ptr<vector<Particle*>> _particleList;
	vector<AttackCollision*> _attack;
	EnemyCharacter* _enemyObject = nullptr;
	bool _isStart = false;
};