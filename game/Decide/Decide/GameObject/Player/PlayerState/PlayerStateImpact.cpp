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
	movespeed.y = _Player->GetCharaCon().GetMoveSpeed().y;

	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();

	if (_Player->_Anim->GetPlaying() != true)
	{
		_Player->ChangeState(Player::State::Idol);
		return;
	}


}

void PlayerStateImpact::Enter()
{

}

void PlayerStateImpact::Leave()
{

}