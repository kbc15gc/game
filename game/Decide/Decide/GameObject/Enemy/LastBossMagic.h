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
	//			�u���X���L�����N�^�[�̐��ʂ����]������N�H�[�^�j�I���B
	//			�u���X���x�B
	//			���_���[�W�{��(�p�[�Z���g)�B
	//			�����_���[�W�{��(�p�[�Z���g)�B
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal,const Quaternion& rot,const float speed,int firePower = 100,int bombPower = 100);

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void _BreathStartSubClass()override;

	// �u���X���ˏI���B
	inline void _BreathEndSubClass()override {
	}

	inline void Shot() {
		_isShot = true;
		_attack[0]->SetIsHit(false);
	}


	void BreathStop()override;

	void BreathPlay()override;

private:

	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _initParticleParam;
	Vector3 _initEmitPos = Vector3::zero;
	Vector3 _direction = Vector3::zero;
	float _speed = 0.0f;
	float _timeCounter;
	float _interval = 3.0f;
	bool _isShot = false;
	bool _isBomb = false;
	float _margin = 1.0f;
	Quaternion _rotation;
	int _firePower;
	int _bombPower;
};