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

	// �u���X�I�u�W�F�N�g��^�����p�����[�^�ō쐬�B
	// �����F	�u���X���o���L�����N�^�[�B
	//			�u���X�����ʒu(���[�J�����W�A�e�̓L�����N�^�[��Transform)�B
	//			�u���X�̃X�s�[�h�B
	//			�u���X�̗��q�����҂����ԁB
	//			�ǂ̎��ŉ񂷂��B
	//			��]��(�L�����N�^�[�̐��ʂ��牽�x�񂷂�)�B
	//			��Z�J���[�B
	//			�U���{���B
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal,float power,float interval, const Vector3& axis,float deg,const char* fileName = "t1.png",const Vector2& size = Vector2(0.5f, 0.5f), const float brightness = 2.0f,Color mul = Color::white,int atk = 100);

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void _BreathStartSubClass()override;

	// �u���X���ˏI���B
	inline void _BreathEndSubClass()override {
		testBreathEnd = true;
		_isCreateBreathEnd = true;
		_particleEmitter->SetEmitFlg(false);
		_particleEmitter->AchievedCreateParticleEnd();
		_particleEmitter->SetActive(false);
	}

	void BreathStop()override;

	void BreathPlay()override;


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
	float _power;
	int _atk;
	
public:
	bool testBreathEnd = false;
};