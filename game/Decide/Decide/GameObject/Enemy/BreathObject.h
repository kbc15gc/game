#pragma once

#include "fbEngine\fbstdafx.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "AttackCollision.h"
#include "fbEngine\_Object\_GameObject\Particle.h"

class EnemyCharacter;

// �u���X�I�u�W�F�N�g�B
// (�u���X���ˏ������I���������u���X�̋������Ǘ��ł���悤�ɂ��邽�߂ɃN���X������)�B
class BreathObject :
	public GameObject
{
public:
	// �����̓I�u�W�F�N�g�̖��O�B
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

	// �u���X���ˊJ�n�B
	inline void BreathStart() {
		_isStart = true;
		_BreathStartSubClass();
	}

	// �u���X���ˏI���B
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
	// �������B
	void Create(EnemyCharacter* enemy) {
		_enemyObject = enemy;
	}

	virtual void _BreathStartSubClass() = 0;

	virtual void _BreathEndSubClass() = 0;

private:
	// �Փ˔���R���W�����̍X�V�B
	virtual void _UpdateCollision() {};

protected:
	unique_ptr<vector<Particle*>> _particleList;
	vector<AttackCollision*> _attack;
	EnemyCharacter* _enemyObject = nullptr;
	bool _isStart = false;
};