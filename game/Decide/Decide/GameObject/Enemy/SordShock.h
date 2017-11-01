#pragma once

#include "GameObject\Enemy\BreathObject.h"

// �{�X�̌��U���̏Ռ��g�B
class SordShock :
	public BreathObject
{
public:
	// �����̓I�u�W�F�N�g�̖��O�B
	SordShock(const char* name) :BreathObject(name) {

	}
	~SordShock() {
		if (_tirotiroEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(_tirotiroEmitter);
		}
		if (_shockParticleEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(_shockParticleEmitter);
		}
	}

	// �u���X�I�u�W�F�N�g�������B
	// �����F	�u���X���o���L�����N�^�[�B
	//			�u���X�����ʒu(���[�J�����W�A�e�̓L�����N�^�[��Transform)�B
	//			�Ռ��g�̐i�s���x�B
	//			�Ռ��g�̔򋗗��B
	void Create(EnemyCharacter* obj, const Vector3& emitPosLocal, const Vector3& speed,const float range);

	void Awake()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void _BreathStartSubClass()override;

	// �u���X���ˏI���B
	inline void _BreathEndSubClass()override {
	}

private:

	ParticleEmitter* _shockParticleEmitter = nullptr;
	ParticleParameter _initShockParticleParam;
	Vector3 _emitPos = Vector3::zero;
	Vector3 _speed = Vector3::zero;
	float _range = 0.0f;	// �Ռ��g�̔򋗗��B


	ParticleEmitter* _tirotiroEmitter = nullptr;
	ParticleParameter _initTirotiroParticleParam;

	bool _isShot = false;
	bool _isBomb = false;
	bool _isAutoRange = false;

	// ���낿�낪�c�鎞�ԁB
	float _timeCounter;
	float _interval = 15.0f;
};