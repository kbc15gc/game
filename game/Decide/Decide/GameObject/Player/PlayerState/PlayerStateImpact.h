#pragma once
#include "PlayerState.h"

//外部からプレイヤーを止めるときにつかわれる。

class PlayerStateImpact : public PlayerState
{
public:
	PlayerStateImpact(Player* player);
	~PlayerStateImpact();
	void Update()override;
	void Enter()override;
	void Leave()override;
};