#pragma once
#include "PlayerState.h"

class PlayerStateRun : public PlayerState
{
public:
	PlayerStateRun(Player* player);
	~PlayerStateRun();
	void Update()override;
	void Enter()override;
	void Leave()override;
};