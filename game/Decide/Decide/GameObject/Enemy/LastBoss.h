#pragma once
#include "EnemyCharacter.h"
#include "GameObject\Enemy\LaserBreath.h"


// �p���N���X�B
// �{�X�G�l�~�[(���X�{�X)�B
class LastBoss :
	public EnemyCharacter
{
public:
	enum class LastBossState { LastBossThrone = static_cast<int>(State::Death) + 1, LastBossMagician, LastBossHistory, LastBossDown };

private:
	// �G�l�~�[(���X�{�X)�̃A�j���[�V�����ԍ��B
	enum class AnimationLastBoss {
		Move = 0,
		SordAttack,
		Magic,
		Max,
	};

public:
	LastBoss(const char* name);
	~LastBoss();

	void SordAttackEvent();
	void SordAttackEvent2();
	void MagicAttackStart1();
	void MagicAttackEnd1();
	void MagicAttackStart2();
	void MagicAttackEnd2();
	void MagicAttackStart3();
	void MagicAttackEnd3();

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

	// ��΂ɑ��̃N���X�ł��g��Ȃ��X�e�[�g�͂������ɓo�^�B
	void _BuildStateSubClass()override;

	// �A�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _sordAttack;	// �P�U�������B
	unique_ptr<EnemyBreathAttack> _magicAttack;

	LaserBreath* _sordAttackLaser = nullptr;
};