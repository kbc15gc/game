#pragma once

#include "BreathObject.h"


class LaserBreath :
	public BreathObject
{
public:
	// �����̓I�u�W�F�N�g�̖��O�B
	LaserBreath(const char* name) :BreathObject(name) {

	}
	~LaserBreath() {
		INSTANCE(GameObjectManager)->AddRemoveList(_particleEmitter);
	}

	// �u���X�I�u�W�F�N�g�������B
	// �����F	�u���X���o���L�����N�^�[�B
	//			�u���X�����ʒu(���[�J�����W�A�e�̓L�����N�^�[��Transform)�B
	void Init(EnemyCharacter* obj, const Vector3& emitPosLocal)override;

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void BreathStart();

	// �u���X���ˏI���B
	inline void BreathEnd()override {
		_particleEmitter->SetEmitFlg(false);
		_particleEmitter->AchievedCreateParticleEnd();
		_particleEmitter->SetActive(false);
	}

private:
	// �Փ˔���R���W�����̍X�V�B
	void _UpdateCollision()override;


private:
	Particle* _start = nullptr;	// �U�����A�ŏ��ɐ������ꂽ�p�[�e�B�N��(�u���X�̐擪)�B
	Particle* _end = nullptr;	// �U�����A�Ō�ɐ������ꂽ�p�[�e�B�N��(�u���X�̏I�[)�B
	Vector3 _breathScale;

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
};