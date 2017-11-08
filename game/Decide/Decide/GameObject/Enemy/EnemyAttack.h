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
	void Init(float attackRange, int animType = -1, float interpolate = 0.0f,float playSpeed = 1.0f, int animLoopNum = 1, int playEventNo = 0);
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
		_breath = obj;
		_breath->SetActive(true);
		_breath->BreathStart();
	}

	// �u���X�I���B
	inline void BreathEnd() {
		if (_breath) {
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
};

class GhostComboAttack:public EnemyAttack{
private:
	enum WarpState{Through, Materialization};
public:
	GhostComboAttack(EnemyCharacter* object) :EnemyAttack(object) {
		_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
		_oneCombo.reset(new EnemySingleAttack(object));
	}
	~GhostComboAttack() {};

	void Entry()override;

	bool Update()override;

	void Exit()override {
		_enemyObject->SetAlpha(1.0f);
	}

private:
	Player* _player = nullptr;
	unique_ptr<EnemySingleAttack> _oneCombo;
	int _attackNum = 4;
	int _comboCount;
	WarpState _nowWarpState;

	//const float _chaceTime = 0.4f;
	const float _chaceTime = 0.45f;

	float _counter = 0.0f;
	bool _isWarpEnd = false;	// ���̉��������B
	bool _isAttackEnd = false;
};