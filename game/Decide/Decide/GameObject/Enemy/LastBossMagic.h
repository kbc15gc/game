#pragma once

#include "GameObject\Enemy\BreathObject.h"

class LastBossMagic :
	public BreathObject
{
public:
	// �����̓I�u�W�F�N�g�̖��O�B
	LastBossMagic(const char* name) :BreathObject(name) {

	}
	~LastBossMagic() {
		INSTANCE(GameObjectManager)->AddRemoveList(_particleEmitter);
	}

	// �u���X�I�u�W�F�N�g�������B
	// �����F	�u���X���o���L�����N�^�[�B
	//			�u���X�����ʒu(���[�J�����W�A�e�̓L�����N�^�[��Transform)�B
	//			�u���X���x(�����ƃX�s�[�h������)�B
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal,const Vector3& speed);

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void _BreathStartSubClass()override;

	// �u���X���ˏI���B
	inline void _BreathEndSubClass()override {
	}

	inline void Shot() {
		_isShot = true;
	}

private:

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
	Vector3 _initEmitPos = Vector3::zero;
	Vector3 _direction = Vector3::zero;
	Vector3 _speed = Vector3::zero;
	float _timeCounter;
	float _interval = 3.0f;
	bool _isShot = false;
	bool _isBomb = false;
	float _margin = 1.0f;
};