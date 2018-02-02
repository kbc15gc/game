#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "Scene\GameScene.h"

class LastBossMagic;
class SordShock;

// �p���N���X�B
// �{�X�G�l�~�[(���X�{�X)�B
class LastBoss :
	public EnemyCharacter
{
public:
	enum class LastBossState { LastBossThrone = static_cast<int>(State::Death) + 1, LastBossMagician, LastBossHistory, LastBossDown };

	enum class LastBossSoundIndex { Fire2 = static_cast<int>(EnemyCharacter::SoundIndex::Max),Fire3,Shot1, Shot2,Shot3,Danmatuma,Cyouhatu,MagicianStart,Warau,ChargeSord,WarpSord, ChargeFire,Damage/*,Battle1,Battle2*/};

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


	void ChargeSordSpeed();
	void SordAttackEvent();
	void SordAttackEvent2();
	void SordAttackEvent3();
	void SordAttackEvent4();
	void FastSord();
	void FastSord2();
	void MagicAttackSpeed1();
	void MagicAttackStart1();
	void MagicAttackShot1();
	void MagicAttackStart2();
	void MagicAttackShot2();
	void MagicAttackStart3();
	void MagicAttackShot3();

	void HighMagicAttackStart1();
	void HighMagicAttackShot1();
	void HighMagicAttackStart2();
	void HighMagicAttackShot2();
	void HighMagicAttackStart3();
	void HighMagicAttackShot3();
	void HighMagicAttackStart4();
	void HighMagicAttackShot4();
	void HighMagicAttackStart5();
	void HighMagicAttackShot5();
	void HighMagicAttackStart6();
	void HighMagicAttackShot6();

	void Danmatuma();

	void BuffEvent();
	void DebuffEvent();
	void EntourageCommand();
	void EncourageBuff();
	void Special();

	void PlayVoiceCyouhatu();
	void PlayVoiceMagicStart();
	void PlayVoiceWarau();
	void PlayVoiceChargeSord();
	void PlayVoiceWarpSord();
	void PlayVoiceChargeFire();
	void PlayVoiceDamage();

#ifdef _DEBUG
	void Debug()override;
#endif // _DEBUG

	inline void SetSaveState(LastBossState state) {
		_saveState = state;
	}

	inline EnemySingleAttack* GetSpecialAttack()const {
		return _specialAttack.get();
	}

	inline EnemySingleAttack*GetEncourageBuffAttack()const {
		return _encourageBuffAttack.get();
	}

	// �o�g���I���B
	void BattleEnd() {
		if (_isStartBattle) {
			static_cast<GameScene*>(INSTANCE(SceneManager)->GetNowScene())->ResetBGMIndex();
			_ChangeState(_initState);
			_isStartBattle = false;
		}
	}

	void SetActive(const bool act, const bool children = false)override;

	void SetIsStartBattle(bool flg) {
		_isStartBattle = flg;
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
	unique_ptr<EnemySingleAttack> _sordAttack2;	// �P�U�������B
	unique_ptr<EnemyWarpAttack> _warpAttack;
	unique_ptr<EnemyBreathAttack> _magicAttack;
	unique_ptr<EnemyBreathAttack> _highMagicAttack;

	unique_ptr<EnemySingleAttack> _buffAttack;
	unique_ptr<EnemySingleAttack> _debuffAttack;
	unique_ptr<EnemySingleAttack> _commandAttack;
	unique_ptr<EnemySingleAttack> _encourageBuffAttack;
	unique_ptr<EnemySingleAttack> _specialAttack;


	LastBossMagic* _magicFire1 = nullptr;
	LastBossMagic* _magicFire2 = nullptr;
	LastBossMagic* _magicFire3 = nullptr;

	LastBossMagic* _magicBigFire1 = nullptr;
	LastBossMagic* _magicBigFire2 = nullptr;
	LastBossMagic* _magicBigFire3 = nullptr;
	LastBossMagic* _magicBigFire4 = nullptr;
	LastBossMagic* _magicBigFire5 = nullptr;
	LastBossMagic* _magicBigFire6 = nullptr;

	SordShock* _sordAttackShot0 = nullptr;
	SordShock* _sordAttackShot1 = nullptr;
	SordShock* _sordAttackShot2 = nullptr;

	bool _isStartBattle = false;

	GameScene* _scene = nullptr;
};