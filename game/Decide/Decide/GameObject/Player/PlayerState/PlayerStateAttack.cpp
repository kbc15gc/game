#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

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

	//UŒ‚Žž‚Ì‚ ‚½‚è”»’èì¬
	//Œ»Ý‚ÌƒtƒŒ[ƒ€Žæ“¾
	const int nowFrame = player->_Anim->NowFrame();
	//ƒtƒŒ[ƒ€‚ª10‚ÌŽž‚ ‚½‚è”»’èì¬
	if (nowFrame == 10)
	{
		//UŒ‚ƒRƒŠƒWƒ‡ƒ“ì¬
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		Transform* trans = attack->GetComponent<Transform>();
		trans->SetLocalPosition(player->transform->Local(Vector3(0.0f, 1.0f, 0.0f)));
		trans->SetLocalAngle(player->transform->GetLocalAngle());
		trans->UpdateTransform();
		attack->Create(Vector3(1.0f,1.0f,1.0f),AttackCollision::CollisionMaster::Player,-1.0f);
	}

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