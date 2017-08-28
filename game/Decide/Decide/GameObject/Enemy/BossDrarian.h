#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"

class EnemyBreathAttack;

// �p���N���X�B
// �{�X�G�l�~�[(���s�^�h���S��)�B
class BossDrarian :
	public EnemyCharacter
{
private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationDrarian {
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
	void CreateAttackCollision_TailAttack1();
	void CreateAttackCollision_TailAttack2();
	void CreateAttackCollision_TailAttack3();
	void CreateAttackCollision_TailAttack4();
	void AnimationEvent_BreathStart();
	void AnimationEvent_BreathEnd();

protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* AttackSelect()override;

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	void _ConfigCharacterController()override;

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;
private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// �P�U������(1�̃N���X���G�l�~�[�̎�ʂȂ̂ŁA�ÓI�����o�ŃI�b�P�[�����ǃG���[�͂���������)�B
	unique_ptr<EnemySingleAttack> _tailAttack;
	unique_ptr<EnemyBreathAttack> _breathAttack;
};


class EnemyBreathAttack : public EnemyAttack {
public:
	EnemyBreathAttack(EnemyCharacter* object);

	// �p�[�e�B�N���̃p�����[�^�[��ݒ�B
	void ConfigParticleParameter(ParticleParameter param) {
		_particleEmitter->Init(param);
	}

	void Start()override {
		_enemyObject->LookAtObject(*_player);
		_particleEmitter->ResetInitVelocity(_enemyObject->transform->GetForward() * _particleEmitter->GetInitVelocity().Length());	// �p�[�e�B�N���̔�ԕ��������˂݁[�̌����ɍĐݒ�B
	};
	bool Update()override;

	void Exit()override {
		BreathEnd();
	};


	// �u���X�J�n�B
	inline void BreathStart() {
		_particleEmitter->SetEmitFlg(true);
	}

	// �u���X�I���B
	inline void BreathEnd() {
		_particleEmitter->SetEmitFlg(false);
	}
private:
	GameObject* _player = nullptr;
	ParticleEmitter* _particleEmitter = nullptr;
	ParticleParameter _particleParam;
};