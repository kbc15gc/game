#pragma once
#include "PlayerState.h"

class PlayerStateDeath : public PlayerState
{
public:
	PlayerStateDeath(Player* player);
	~PlayerStateDeath();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
#pragma once
