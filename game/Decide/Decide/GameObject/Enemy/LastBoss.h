#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"

class LastBossMagic;
class SordShock;

// �p���N���X�B
// �{�X�G�l�~�[(���X�{�X)�B
class LastBoss :
	public EnemyCharacter
{
public:
	enum class LastBossState { LastBossThrone = static_cast<int>(State::Death) + 1, LastBossMagician, LastBossHistory, LastBossDown };

	// �G�l�~�[(���X�{�X)�̃A�j���[�V�����ԍ��B
	enum class AnimationLastBoss {
		Wait = 0,
		SordAttack,
		Magic,
		Damage,
		Move,
		Death,
		WaitThrone,	// �ʍ��ҋ@�B
		MoveThrone,	// �ʍ��ړ��B
		MagicThrone,	// �ʍ����@�B
		ThroneEnd,	// �ʍ��폜�B
		HistoryStart,	// �喂�@��o���B
		//HistoryCharge,	// �喂�@�r���B

		Max,
	};

private:
public:
	LastBoss(const char* name);
	~LastBoss();


	void SordAttackEvent();
	void SordAttackEvent2();
	void MagicAttackStart1();
	void MagicAttackShot1();
	void MagicAttackStart2();
	void MagicAttackShot2();
	void MagicAttackStart3();
	void MagicAttackShot3();
	void BuffEvent();
	void DebuffEvent();
	void EntourageCommand();
	void EncourageBuff();

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

	inline void SetSaveState(LastBossState state) {
		_saveState = state;
	}

protected:
	void _EndNowStateCallback(State EndStateType)override;

	// �X�e�[�g���؂�ւ�����Ƃ��ɌĂ΂��R�[���o�b�N�B
	// �����F	�؂�ւ��O�̃X�e�[�g�^�C�v�B
	//			�؂�ւ������̃X�e�[�g�^�C�v�B
	// ���������̂͌p����Ŏ����B
	void _ChangeStateCallback(State prev, State next)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* _AttackSelectSubClass()override;

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	void _ConfigCharacterController()override;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�p�̍��̍쐬�֐��B
	// �R���W����������`��Ȃǂ�ݒ肵�A�쐬����B
	void _CreateExtrudeCollision()override;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::ENEMY);
	}

	// ��΂ɑ��̃N���X�ł��g��Ȃ��X�e�[�g�͂������ɓo�^�B
	void _BuildStateSubClass()override;

	// �A�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// �����F	�A�j���[�V�����I�����Ԃ̊i�[�p�z��(���̔z��ɏI�����Ԃ�ݒ肷��A�Y�����̓��f���ɐݒ肳��Ă���A�j���[�V�����ԍ�)�B
	// �󂯎��z����̒l�̓f�t�H���g��-1�ƂȂ��Ă���̂ŁA�A�j���[�V�����̏I�����Ԃ�1�b�ȏ�̂��̂͐ݒ肵�Ȃ��Ă悢�B
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	inline void _DropSubClass()override {
	}


private:
	LastBossState _saveState;
	unique_ptr<EnemySingleAttack> _sordAttack;	// �P�U�������B
	unique_ptr<EnemyBreathAttack> _magicAttack;
	unique_ptr<EnemySingleAttack> _buffAttack;
	unique_ptr<EnemySingleAttack> _debuffAttack;
	unique_ptr<EnemySingleAttack> _commandAttack;
	unique_ptr<EnemySingleAttack> _encourageBuffAttack;

	LastBossMagic* _magicFire1 = nullptr;
	LastBossMagic* _magicFire2 = nullptr;
	LastBossMagic* _magicFire3 = nullptr;

	SordShock* _sordAttackShot0 = nullptr;
	SordShock* _sordAttackShot1 = nullptr;
	SordShock* _sordAttackShot2 = nullptr;
};