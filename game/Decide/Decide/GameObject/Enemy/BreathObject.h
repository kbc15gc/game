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
		ReleaseCollisionAll();
		ReleaceParticleAll();
	}

	// �������B
	void Init(EnemyCharacter* enemy) {
		_enemyObject = enemy;
	}

	// ���̃u���X���`������p�[�e�B�N����o�^����z��̃|�C���^��ݒ�B
	// �����F	�p�[�e�B�N���z��̃|�C���^�B
	// �����̊֐����Ă񂾂����ł̓G�~�b�g�͊J�n����Ȃ��B
	// ���G�~�b�g����ۂ̓G�~�b�^�[��SetEmitFlg�֐����ĂԁB
	void SetParticleList(unique_ptr<vector<Particle*>> list) {
		if (list) {
			_particleList = move(list);
		}
	}

	// �u���X���ˊJ�n�B
	virtual inline void BreathStart() = 0;

	// �u���X���ˏI���B
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
	// �Փ˔���R���W�����̍X�V�B
	virtual void _UpdateCollision() = 0;

protected:
	unique_ptr<vector<Particle*>> _particleList;
	vector<AttackCollision*> _attack;
	EnemyCharacter* _enemyObject = nullptr;
};