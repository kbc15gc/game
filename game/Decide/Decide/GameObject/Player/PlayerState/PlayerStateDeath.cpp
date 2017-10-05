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
		//タイトルシーンへ移行
		//INSTANCE(SceneManager)->ChangeScene("TitleScene");
		/*_Player->transform->SetLocalPosition(_Player->_StartPos);
		_Player->ChangeState(Player::State::Idol);
		_Player->_PlayerParam->AddParam(CharacterParameter::HP, _Player->_PlayerParam->GetParam(CharacterParameter::MAXHP));
		_Player->_HPBar->AddValue(_Player->_PlayerParam->GetParam(CharacterParameter::MAXHP));*/
		_Player->Releace();
		INSTANCE(SceneManager)->ChangeScene("TitleScene",true);
	}
}

void PlayerStateDeath::Enter()
{

}

void PlayerStateDeath::Leave()
{

}