#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateStop.h"
#include "GameObject\Component\ParameterBar.h"


PlayerStateStop::PlayerStateStop(Player* player) :
	PlayerState(player)
{

}

PlayerStateStop::~PlayerStateStop()
{

}


void PlayerStateStop::Update()
{
	//ˆÚ“®‘¬“x
	Vector3 movespeed = Vector3::zero;
	movespeed.y = _Player->GetCharaCon().GetMoveSpeed().y;

	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();
}

void PlayerStateStop::Enter()
{

}

void PlayerStateStop::Leave()
{

}