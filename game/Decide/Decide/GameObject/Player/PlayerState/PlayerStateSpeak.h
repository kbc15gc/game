#pragma once
#include "PlayerState.h"

//�O������v���C���[���~�߂�Ƃ��ɂ�����B

class PlayerStateSpeak : public PlayerState
{
public:
	PlayerStateSpeak(Player* player);
	~PlayerStateSpeak();
	void Update()override;
	void Enter()override;
	void Leave()override;
};
