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
		//_Player->Releace();
		INSTANCE(SceneManager)->ChangeScene("TitleScene", true);
	}
	if (_DeadColor.a < 1.0f)
	{
		_DeadImage->SetBlendColor(Color(_DeadColor.r, _DeadColor.g, _DeadColor.b, _DeadColor.a += 0.02f));
	}
}

void PlayerStateDeath::Enter()
{
	_DeadImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("DeadImage", 9);
	_DeadImage->SetTexture(LOADTEXTURE("GameOver.png"));
	_DeadImage->SetPivot(0.0f, 0.0f);
	_DeadImage->SetSize({ WindowW, WindowH });

	_Time = 0.0f;
	_DeadColor = { 1.0f,1.0f,1.0f,0.0f };
	_DeadImage->SetBlendColor(_DeadColor);
}

void PlayerStateDeath::Leave()
{

}