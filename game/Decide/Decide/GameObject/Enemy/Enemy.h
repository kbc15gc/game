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
protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;
};

