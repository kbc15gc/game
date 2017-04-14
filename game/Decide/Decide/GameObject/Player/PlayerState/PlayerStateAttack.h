#pragma once
#include "PlayerState.h"

class PlayerStateAttack : public PlayerState
{
public:
	PlayerStateAttack(Player* player);
	~PlayerStateAttack();
	void Update()override;
	void Enter()override;
	void Leave()override;
};