#pragma once
#include "PlayerState.h"

//�O������v���C���[���~�߂�Ƃ��ɂ�����B

class PlayerStateImpact : public PlayerState
{
public:
	PlayerStateImpact(Player* player);
	~PlayerStateImpact();
	void Update()override;
	void Enter()override;
	void Leave()override;
};