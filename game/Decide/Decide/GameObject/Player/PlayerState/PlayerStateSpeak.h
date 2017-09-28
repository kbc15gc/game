#pragma once
#include "PlayerState.h"

//外部からプレイヤーを止めるときにつかわれる。

class PlayerStateSpeak : public PlayerState
{
public:
	PlayerStateSpeak(Player* player);
	~PlayerStateSpeak();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
