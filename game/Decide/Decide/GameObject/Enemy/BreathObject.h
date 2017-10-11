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
		ReleaseCollisionAll();
		ReleaceParticleAll();
	}

	// 初期化。
	void Init(EnemyCharacter* enemy) {
		_enemyObject = enemy;
	}

	// このブレスを形成するパーティクルを登録する配列のポインタを設定。
	// 引数：	パーティクル配列のポインタ。
	// ※この関数を呼んだだけではエミットは開始されない。
	// ※エミットする際はエミッターのSetEmitFlg関数を呼ぶ。
	void SetParticleList(unique_ptr<vector<Particle*>> list) {
		if (list) {
			_particleList = move(list);
		}
	}

	// ブレス発射開始。
	virtual inline void BreathStart() = 0;

	// ブレス発射終了。
	virtual inline void BreathEnd() = 0;

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

private:
	// 衝突判定コリジョンの更新。
	virtual void _UpdateCollision() = 0;

protected:
	unique_ptr<vector<Particle*>> _particleList;
	vector<AttackCollision*> _attack;
	EnemyCharacter* _enemyObject = nullptr;
};