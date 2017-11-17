#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

// �p���N���X�B
// �{�X�G�l�~�[(���߃S�[�X�g)�B
class BossGhost :
	public EnemyCharacter
{
public:
	enum class BossGhostState { BossGhostPairAttack = static_cast<int>(State::Death) + 1};

	enum PairAttackType{Combo = 0,Laser,Max};

private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationBossGhost {
		Wait = 0,
		Walk,
		Attack,
		Dance,
		Damage,
		Death,
		Max
	};
public:
	BossGhost(const char* name);
	~BossGhost();

	// �A�j���[�V�����C�x���g�֘A�B
	void CreateCollision();
	void LaserStartSingle();
	void LaserEndSingle();
	void LaserStart();
	void LaserEnd();


	static void SelectPairAttack() {
		_selectNowPairAttack = static_cast<PairAttackType>(rand() % PairAttackType::Max);
	}

	inline void SetIsCommand(bool flg) {
		_isCommand = flg;
	}

	inline void SetPairGhost(BossGhost* ghost) {
		_pairGhost = ghost;
	}

	inline void SetIntervalStartPairAttack(float time) {
		_intervalStartPairAttack = time;
	}

	// �����U���̏����������B
	inline bool GetIsPairAttackReady()const {
		return _isPairAttackReady;
	}

	inline EnemyAttack* GetNowPairAttack() {
		return _pairAttackArray[_selectNowPairAttack];
	}
protected:
	void _EndNowStateCallback(State EndStateType)override;

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

	void _BuildStateSubClass();

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::ENEMY);
	}

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// �����F	�A�j���[�V�����I�����Ԃ̊i�[�p�z��(���̔z��ɏI�����Ԃ�ݒ肷��A�Y�����̓��f���ɐݒ肳��Ă���A�j���[�V�����ԍ�)�B
	// �󂯎��z����̒l�̓f�t�H���g��-1�ƂȂ��Ă���̂ŁA�A�j���[�V�����̏I�����Ԃ�1�b�ȏ�̂��̂͐ݒ肵�Ȃ��Ă悢�B
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Oil, transform->GetPosition() + Vector3(0.0f, -1.5f, 0.0f));
	}

private:
	State _saveState;
	unique_ptr<EnemyComboAttack> _comboAttack;	// �P�U������(1�̃N���X���G�l�~�[�̎�ʂȂ̂ŁA�ÓI�����o�ŃI�b�P�[�����ǃG���[�͂���������)�B
	unique_ptr<EnemyComboAttack> _laserComboAttack;
	unique_ptr<EnemySingleAttack> _singleAttack;
	unique_ptr<EnemyBreathAttack> _singleLaser;

	bool _isCommand = false;

	float _intervalStartPairAttack;
	bool _isPairAttackReady = false;	// �y�A�U���̏����������B
	BossGhost* _pairGhost = nullptr;	// �����B

	static PairAttackType _selectNowPairAttack;	// ���݂̃y�A�U���������B
	vector<EnemyAttack*> _pairAttackArray;
};
