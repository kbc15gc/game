#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateDeath.h"

PlayerStateDeath::PlayerStateDeath(Player* player) :
	PlayerState(player)
{
}

PlayerStateDeath::~PlayerStateDeath()
{

}


void PlayerStateDeath::Update()
{
	if (_Player->_Anim->GetPlaying() != true)
	{
		//�^�C�g���V�[���ֈڍs
		_Player->Releace();
		INSTANCE(SceneManager)->ChangeScene("GameScene");
	}
}

void PlayerStateDeath::Enter()
{

}

void PlayerStateDeath::Leave()
{

}