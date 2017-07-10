#pragma once
#include "EnemyCharacter.h"

// �p���N���X�B
// �{�X�G�l�~�[(���s�^�h���S��)�B
class BossDrarian :
	public EnemyCharacter
{
private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationDrarian {
		Wait = 0,
		Max
	};
public:
	BossDrarian(const char* name);
	~BossDrarian();
protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyCharacter::State AttackSelect()override;

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

private:
	State _saveState;
};

