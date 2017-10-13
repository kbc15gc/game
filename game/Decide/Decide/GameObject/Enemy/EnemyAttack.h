#pragma once

#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Enemy\BreathObject.h"
#include "GameObject\Enemy\EnemyCharacter.h"


// �G�l�~�[�̍U�������B
// ���U���̎�ނ��Ƃɍ쐬�B
// ���p����Ŋe�֐��������B
// �����̃N���X�̃C���X�^���X���U���X�e�[�g�ɓn�����ƂŊe���߂��Ăяo�����B
// �A�j���[�V�������ݒ肳��Ă���Ύ����ōĐ������B
class EnemyAttack {
public:
	EnemyAttack(EnemyCharacter* object) {
		_enemyObject = object;
	}
	virtual ~EnemyAttack() {

	}

	// �������֐��B
	// �����F	�U���\�͈́B
	//			�Đ�����A�j���[�V�����̎��(�����l�͍Đ����Ȃ�,���f�����Ƃ̃A�j���[�V�����ԍ��ŁA�e�[�u���̔ԍ��ł͂Ȃ�)�B
	//			�A�j���[�V������Ԏ���(�����l��0)�B
	//			�A�j���[�V�������[�v�Đ���(1�Ń��[�v�Ȃ��A-1�Ŗ������[�v)�B
	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, int animLoopNum = 1);
	virtual void Entry() = 0;	// �U���X�e�[�g�̍ŏ��̍X�V�O�Ɉ�x�����Ă΂�鏈���B
	virtual bool Update() = 0;	// �U���X�e�[�g�̍X�V�����ŌĂяo����鏈��(�߂�l�͏I��������)�B
	virtual void Exit() = 0;	// �U���X�e�[�g�I�����ɌĂяo����鏈���B

	// ���̍U�����̃X�e�[�g�J�ڂ������邩�B
	// �����F	�؂�ւ��悤�Ƃ��Ă���X�e�[�g�B
	// ���_���[�W���[�V�����̍Đ����֘A�����ɂ��悤���Ǝv�������A�W�����v�U���Ȃǂ̎��s���ɗ����X�e�[�g�ɑJ�ڂ��Ȃ��悤���䂷�邱�Ƃ��l�������B
	inline virtual bool EnemyAttack::IsPossibleChangeState(EnemyCharacter::State next)const {
		return true;
	}


	inline const int GetAnimationType()const {
		return _animType;
	}
	inline const float GetInterpolate()const {
		return _interpolate;
	}
	inline const int GetAnimationLoopNum() const {
		return _animLoopNum;
	}
	inline void SetIsPlaying(bool flg) {
		_isPlaying = flg;
	}
	inline float GetAttackRange()const {
		return _AttackRange;
	}
protected:
	int _animType = -1;	// �Đ�����A�j���[�V�����̎��(�����l�͍Đ����Ȃ�,���f�����Ƃ̃A�j���[�V�����ԍ��ŁA�e�[�u���̔ԍ��ł͂Ȃ�)�B
	float _interpolate = 0.0f;	// �A�j���[�V������Ԏ���(�����l��0)�B
	int _animLoopNum = 1;	// �A�j���[�V�������[�v�Đ���(1�Ń��[�v�Ȃ��A-1�Ŗ������[�v)�B
	bool _isPlaying = false;	// �A�j���[�V�����Đ������̃t���O(�X�V�������ɍU���X�e�[�g����ݒ肳���)�B
	float _AttackRange = 0.0f;	// �U���\�͈́B
	EnemyCharacter* _enemyObject = nullptr;
};

// ���P�U��(�U�����[�V������񕪍U��)�B
class EnemySingleAttack :public EnemyAttack {
public:
	EnemySingleAttack(EnemyCharacter* object);
	void Entry()override;
	bool Update()override;

	void Exit()override {};

private:
	GameObject* _player = nullptr;
};

class EnemyBreathAttack : public EnemyAttack {
public:
	// �����F	���̃u���X�U�����s���G�l�~�[�B
	EnemyBreathAttack(EnemyCharacter* object);

	~EnemyBreathAttack() {
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		if (_isBreathStart) {
			// �u���X�����˂���Ă���΃u���X�I�u�W�F�N�g�͎����I�ɏ��ł���B
			BreathEnd();
			_isBreathStart = false;
		}
	};

	// �u���X�J�n�B
	// �e���v���[�g�����F	�g�p����u���X�B
	// �����F	�u���X�𔭎˂���L�����N�^�[�B
	//			�u���X�̔����ʒu�B
	template<class T>
	inline void BreathStart(EnemyCharacter* obj, const Vector3& emitPosLocal) {
		_isBreathStart = true;
		_breath = INSTANCE(GameObjectManager)->AddNew<T>("breath", 8);
		_breath->SetActive(true);
		_breath->Init(obj, emitPosLocal);
		_breath->BreathStart();
	}

	// �u���X�I���B
	inline void BreathEnd() {
		if (_breath) {
			_breath->BreathEnd();
			_breath = nullptr;
		}
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// �u���X�I�u�W�F�N�g(�u���X���ˏ������I���������u���X�̋������Ǘ��ł���悤�ɂ��邽�߂ɃN���X������)�B
	bool _isBreathStart = false;
};