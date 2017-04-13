#include "Player.h"
#include "PlayerStateAttack.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{

}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//ˆÚ“®‘¬“xÝ’è
	Vector3 movespeed = player->GetCharaCon().GetMoveSpeed();
	//‹}‚ÉŽ~‚Ü‚ç‚È‚¢‚æ‚¤‚É
	movespeed.Scale(0.3f);
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
	if (player->GetAnimIsPlay() != true)
	{
		player->ChangeState(Player::State::Idol);
	}
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}