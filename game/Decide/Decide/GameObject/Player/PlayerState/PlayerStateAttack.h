#pragma once
#include "PlayerState.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class PlayerStateAttack : public PlayerState
{
public:
	PlayerStateAttack(Player* player);
	~PlayerStateAttack();
	void Update()override;
	void Enter()override;
	void Leave()override;
	//�U���֐�
	//�w�肵���t���[�����ɍU������
	void Attack(int attackframe);
private:
	SoundSource* _SE;
};