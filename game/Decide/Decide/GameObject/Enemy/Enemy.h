#pragma once
#include "EnemyCharacter.h"

// �p���N���X�B
// �G���G�l�~�[(�v���g)�B
class Enemy :
	public EnemyCharacter
{
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

};

