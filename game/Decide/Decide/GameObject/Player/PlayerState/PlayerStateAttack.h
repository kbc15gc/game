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
	//�����]��
	void Dir();
};