#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateImpact.h"
#include "GameObject\Component\ParameterBar.h"


PlayerStateImpact::PlayerStateImpact(Player* player) :
	PlayerState(player)
{

}

PlayerStateImpact::~PlayerStateImpact()
{

}


void PlayerStateImpact::Update()
{
	//ˆÚ“®‘¬“x
	Vector3 movespeed = Vector3::zero;

	if (_Player->_Anim->GetPlaying() != true)
	{
		_Player->ChangeState(Player::State::Idol);
		return;
	}
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();
}

void PlayerStateImpact::Enter()
{

}

void PlayerStateImpact::Leave()
{

}