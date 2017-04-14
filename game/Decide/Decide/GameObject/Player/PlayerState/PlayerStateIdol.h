#pragma once
#include "PlayerState.h"

class PlayerStateIdol : public PlayerState
{
public:
	PlayerStateIdol(Player* player);
	~PlayerStateIdol();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
