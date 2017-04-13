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
		Stand = 0,
		Walk,
		Attack,
		Damage,
		Death,
		Anim
	};
public:
	Enemy(const char* name);
	~Enemy();
	void Awake()override;
	void Start()override;
	void Update()override;
private:
	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;
};

