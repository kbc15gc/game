#pragma once
#include "PlayerState.h"

//外部からプレイヤーを止めるときにつかわれる。

class PlayerStateStop : public PlayerState
{
public:
	PlayerStateStop(Player* player);
	~PlayerStateStop();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
