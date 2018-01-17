#pragma once

#include "GameObject\Player\Player.h"
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
	//			�A�j���[�V�����Đ����x(1.0���傫���l�ōĐ����x�������Ȃ�A�������l�Œx���Ȃ�A�f�t�H���g��1.0)�B
	//			�A�j���[�V�������[�v�Đ���(1�Ń��[�v�Ȃ��A-1�Ŗ������[�v)�B
	//			�g�p����A�j���[�V�����C�x���g�ԍ�(�w�肵���ԍ��̃C�x���g���X�g�ɓo�^���ꂽ�C�x���g���Ăяo�����A�f�t�H���g��0)�B
	virtual void Init(float attackRange, int animType = -1, float interpolate = 0.0f,float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0);
	virtual void Entry();	// �U���X�e�[�g�̍ŏ��̍X�V�O�Ɉ�x�����Ă΂�鏈���B
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
	inline int GetPlayEventNo()const {
		return _playEventNo;
	}
protected:
	int _animType = -1;	// �Đ�����A�j���[�V�����̎��(�����l�͍Đ����Ȃ�,���f�����Ƃ̃A�j���[�V�����ԍ��ŁA�e�[�u���̔ԍ��ł͂Ȃ�)�B
	float _interpolate = 0.0f;	// �A�j���[�V������Ԏ���(�����l��0)�B
	float _playSpeed = 1.0f;
	int _animLoopNum = 1;	// �A�j���[�V�������[�v�Đ���(1�Ń��[�v�Ȃ��A-1�Ŗ������[�v)�B
	int _playEventNo = 0;	// �g�p����A�j���[�V�����C�x���g�ԍ��B
	bool _isPlaying = false;	// �A�j���[�V�����Đ������̃t���O(�X�V�������ɍU���X�e�[�g����ݒ肳���)�B
	float _AttackRange = 0.0f;	// �U���\�͈́B
	EnemyCharacter* _enemyObject = nullptr;

public:
	bool warptest = false;

};

// ���P�U��(�U�����[�V������񕪍U��)�B
class EnemySingleAttack :public EnemyAttack {
public:
	EnemySingleAttack(EnemyCharacter* object);
	void Entry()override;

	// �߂�l�F	�U�����I���������B
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
		if (_breath) {
			if (_breath->GetIsStart()) {
				// �u���X���˒��ɍU�����L�����Z�����ꂽ�B
				BreathEnd();
			}
		}
	};

	// ���̍U���Ɏg�p����u���X�I�u�W�F�N�g�ǉ��B
	// �u���X�U���̂��тɖ���1����ǉ�����B
	// �����F	�g�p����u���X�I�u�W�F�N�g�B
	inline void BreathStart(BreathObject* obj) {
		if (warptest) {
			OutputDebugString("warptest\n");
		}
		_breath = obj;
		_breath->SetActive(true);
		_breath->BreathStart();
	}

	// �u���X�I���B
	inline void BreathEnd() {
		if (_breath) {
			if (warptest) {
				OutputDebugString("warptest\n");
				test = _breath;
				char testc[256];
				sprintf(testc, "test = %p\n", test);
				OutputDebugString(testc);
			}
			_breath->BreathEnd();
			_breath = nullptr;
		}
	}

	inline BreathObject* GetBreathObject() {
		return _breath;
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// �u���X�I�u�W�F�N�g(�u���X���ˏ������I���������u���X�̋������Ǘ��ł���悤�ɂ��邽�߂ɃN���X������)�B
public:
	BreathObject* test = nullptr;
};

class EnemyWarpAttack : public EnemyAttack {
private:
	enum WarpState { Through, Materialization };
public:
	EnemyWarpAttack(EnemyCharacter* object) :EnemyAttack(object) {
		_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	}
	~EnemyWarpAttack() {
	};

	// �A���U���̏������֐��B
	// �����F	�U���\�͈�(�����ł͍U���J�n�\�͈�)�B
	//			�U�������`�����N���X�̃|�C���^(Init�֐����Ă񂾌�̂��̂�n��)�B
	void Init(float attackRange, EnemyAttack* attack) {
		_AttackRange = attackRange;
		_attack.reset(attack);
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		_enemyObject->SetAlpha(1.0f);
		_attack->Exit();
		_isAttackEnd = false;
	}

private:
	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0)override {
		//EnemyAttack::Init(attackRange, animType, interpolate, playSpeed, animLoopNum, playEventNo);
	}

private:
	unique_ptr<EnemyAttack> _attack;
	WarpState _nowWarpState;
	Player* _player = nullptr;
	bool _isWarpEnd = false;	// ���̉��������B
	bool _isAttackEnd = false;
	const float _chaceTime = 0.45f;
	float _counter = 0.0f;
};

class EnemyComboAttack:public EnemyAttack{
public:
	EnemyComboAttack(EnemyCharacter* object) :EnemyAttack(object) {
		_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	}
	~EnemyComboAttack() {
	};

	// �A���U���̏������֐��B
	// �����F	�U���\�͈�(�����ł͍U���J�n�\�͈�)�B
	//			�U�������`�����N���X�̃|�C���^(Init�֐����Ă񂾌�̂��̂�n��)�B
	void Init(float attackRange,EnemyAttack* oneAttack) {
		_AttackRange = attackRange;
		_oneCombo.reset(oneAttack);
	}

	void Entry()override;

	bool Update()override;

	void Exit()override {
		_enemyObject->SetAlpha(1.0f);
		_oneCombo->Exit();
	}


	inline EnemyAttack* GetOneAttack() {
		return _oneCombo.get();
	}
private:

	void Init(float attackRange, int animType = -1, float interpolate = 0.0f, float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0)override {
		//EnemyAttack::Init(attackRange, animType, interpolate, playSpeed, animLoopNum, playEventNo);
	}


private:
	Player* _player = nullptr;
	unique_ptr<EnemyAttack> _oneCombo;
	int _attackNum = 4;
	int _comboCount;
	bool _isStartAttack = false;
	//const float _chaceTime = 0.4f;
};