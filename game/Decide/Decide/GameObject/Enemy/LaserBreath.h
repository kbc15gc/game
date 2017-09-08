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

	}

	void Start()override;

	void Update()override;

	// �u���X���ˊJ�n�B
	void BreathStart();

	// �u���X���ˏI���B
	inline void BreathEnd()override {
	}

private:
	// �Փ˔���R���W�����̍X�V�B
	void _UpdateCollision()override;


private:
	Particle* _start = nullptr;	// �U�����A�ŏ��ɐ������ꂽ�p�[�e�B�N��(�u���X�̐擪)�B
	Particle* _end = nullptr;	// �U�����A�Ō�ɐ������ꂽ�p�[�e�B�N��(�u���X�̏I�[)�B
	Vector3 _breathScale;
};