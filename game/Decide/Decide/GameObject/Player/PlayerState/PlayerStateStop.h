#pragma once
#include "PlayerState.h"

//�O������v���C���[���~�߂�Ƃ��ɂ�����B

class PlayerStateStop : public PlayerState
{
public:
	PlayerStateStop(Player* player);
	~PlayerStateStop();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
