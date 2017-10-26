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
	//			�u���X�̃X�s�[�h�B
	//			�ǂ̎��ŉ񂷂��B
	//			��]��(�L�����N�^�[�̐��ʂ��牽�x�񂷂�)�B
	//			��Z�J���[�B
	void Init(EnemyCharacter* obj, const Vector3& emitPosLocal,float power, const Vector3& axis,float deg, Color mul = Color::white);

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void _BreathStartSubClass()override;

	// �u���X���ˏI���B
	inline void _BreathEndSubClass()override {
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
	Vector3 _velocity;
	float _rad;		// �L�����N�^�[�̐��ʂ���ǂꂭ�炢�񂵂��u���X���B
	Vector3 _axis;	// �ǂ̎��𒆐S�ɉ񂵂����B
	Color _mulColor = Color::white;
};