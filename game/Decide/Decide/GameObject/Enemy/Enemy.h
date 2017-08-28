#pragma once
#include "EnemyCharacter.h"

// �p���N���X�B
// �G���G�l�~�[(�v���g)�B
class Enemy :
	public EnemyCharacter
{
private:
	// �G�l�~�[(�v���g)�̃A�j���[�V�����ԍ��B
	enum class AnimationProt {
		Death = 0,
		Damage,
		Attack,
		Walk,
		Stand,
		Max
	};
public:
	Enemy(const char* name);
	~Enemy();

	void CreateAttackCollision();
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

	// �A�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;
private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// �P�U�������B
};

