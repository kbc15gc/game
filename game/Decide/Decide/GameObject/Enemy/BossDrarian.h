#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

// �p���N���X�B
// �{�X�G�l�~�[(���s�^�h���S��)�B
class BossDrarian :
	public EnemyCharacter
{
private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationBossDrarian {
		Wait = 0,
		Walk,
		Dash,
		Attack,
		Barking,
		TailAttackRight,
		Breath,
		Damage,
		Death,
		Max
	};
public:
	BossDrarian(const char* name);
	~BossDrarian();

	// �A�j���[�V�����C�x���g�֘A�B
	void AnimationEvent_Kamituki();
	void CreateAttackCollision_TailAttackSub1();	// ���̓����Ŏ�_���[�W�B
	void CreateAttackCollision_TailAttackSub2();	// ���̓����Ŏ�_���[�W�B
	void CreateAttackCollision_TailAttackSub3();	// ���̓����Ŏ�_���[�W�B
	void CreateAttackCollision_TailAttackSub4();	// ���̓����Ŏ�_���[�W�B
	void CreateAttackCollision_TailAttack1();
	void CreateAttackCollision_TailAttack2();
	void CreateAttackCollision_TailAttack3();
	void CreateAttackCollision_TailAttack4();
	void AnimationEvent_BreathStart();
	void AnimationEvent_BreathEnd();
	void CreateAttackCollision_BreathAttackSub1();	// ���̓����Ŏ�_���[�W�B
	void CreateAttackCollision_BreathAttackSub2();	// ���̓����Ŏ�_���[�W�B



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

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Stone, transform->GetPosition() + Vector3(0.0f,-1.5f,0.0f));
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// �P�U������(1�̃N���X���G�l�~�[�̎�ʂȂ̂ŁA�ÓI�����o�ŃI�b�P�[�����ǃG���[�͂���������)�B
	unique_ptr<EnemySingleAttack> _tailAttack;
	unique_ptr<EnemyBreathAttack> _breathAttack;
};
