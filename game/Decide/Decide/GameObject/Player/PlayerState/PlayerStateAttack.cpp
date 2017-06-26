#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/PlayerAttack_00.wav");

	//UŒ‚‚P
	{
		AttackCollisionParameter attackparam1;
		attackparam1.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam1.rot = Quaternion::Identity;
		attackparam1.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam1.attackframe = 10.0f;
		attackparam1.lifetime = 0.5f;
		_AttackPram.push_back(attackparam1);
	}
	//UŒ‚‚Q
	{
		AttackCollisionParameter attackparam2;
		attackparam2.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam2.rot = Quaternion::Identity;
		attackparam2.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam2.attackframe = 10;
		attackparam2.lifetime = 0.5f;
		_AttackPram.push_back(attackparam2);
	}
	//UŒ‚‚R
	{
		AttackCollisionParameter attackparam3;
		attackparam3.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam3.rot = Quaternion::Identity;
		attackparam3.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam3.attackframe = 25;
		attackparam3.lifetime = 0.5f;
		_AttackPram.push_back(attackparam3);
	}
	
}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//ˆÚ“®‘¬“xÝ’è
	Vector3 movespeed = _Player->GetCharaCon().GetMoveSpeed();
	//‹}‚ÉŽ~‚Ü‚ç‚È‚¢‚æ‚¤‚É
	movespeed.Scale(0.3f);
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();
	//Œ»Ý‚ÌƒAƒjƒ[ƒVƒ‡ƒ“ƒiƒ“ƒo[
	int currentanimno = _Player->_Anim->GetPlayAnimNo();
	//ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÄ¶‚ªI‚í‚é && ŽŸ‚ÌƒAƒjƒ[ƒVƒ‡ƒ“‚ª‚È‚¢ê‡A‘Ò‹@ó‘Ô‚É•ÏX
	if (_Player->_Anim->GetPlaying() != true && _Player->_NextAttackAnimNo == Player::AnimationNo::AnimationInvalid)
	{
		_Player->ChangeState(Player::State::Idol);
	}
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		|| KeyBoardInput->isPush(DIK_SPACE)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
		//ƒRƒ“ƒ{I
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}
	//‚ ‚½‚è”»’èì¬
 	switch (currentanimno)
	{
		//UŒ‚‚P‚ÌŽž
	case (int)Player::AnimationNo::AnimationAttack01:
		Attack(_AttackPram[0]);
		break;
		//UŒ‚‚Q‚ÌŽž
	case (int)Player::AnimationNo::AnimationAttack02:
		Attack(_AttackPram[1]);
		break;
		//UŒ‚‚R‚ÌŽž
	case (int)Player::AnimationNo::AnimationAttack03:
		Attack(_AttackPram[2]);
		break;
	}
	
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Attack(AttackCollisionParameter pram)
{
	//UŒ‚Žž‚Ì‚ ‚½‚è”»’èì¬
	//Œ»Ý‚ÌƒtƒŒ[ƒ€Žæ“¾
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == pram.attackframe)
	{
		//UŒ‚Žž‚ÌƒTƒEƒ“ƒhÄ¶B
		_SE->Play(false);
		//UŒ‚ƒRƒŠƒWƒ‡ƒ“ì¬
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		attack->Create(_Player->_PlayerParam->GiveDamageMass(),pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, _Player->transform);

		_Player->_AttackValue->transform->SetLocalPosition(Vector3(pram.pos.x,pram.pos.y, pram.pos.z));
		string attackvalue = to_string(_Player->_PlayerParam->GiveDamageMass());
		_Player->_AttackValue->SetString(attackvalue.data());
	}
}