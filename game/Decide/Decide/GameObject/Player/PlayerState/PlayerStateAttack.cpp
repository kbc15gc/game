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

	//攻撃１
	{
		AttackCollisionParameter attackparam1;
		attackparam1.pos = Vector3(0.0f, 0.0f, 1.0f);
		attackparam1.rot = Quaternion::Identity;
		attackparam1.scale = Vector3(2.0f,2.0f,2.0f);
		attackparam1.attackframe = 10.0f;
		attackparam1.lifetime = 0.5f;
		_AttackPram.push_back(attackparam1);
	}
	//攻撃２
	{
		AttackCollisionParameter attackparam2;
		attackparam2.pos = Vector3(0.0f, 0.0f, 1.0f);
		attackparam2.rot = Quaternion::Identity;
		attackparam2.scale = Vector3(2.0f, 2.0f, 2.0f);
		attackparam2.attackframe = 10;
		attackparam2.lifetime = 0.5f;
		_AttackPram.push_back(attackparam2);
	}
	//攻撃３
	{
		AttackCollisionParameter attackparam3;
		attackparam3.pos = Vector3(0.0f, 0.0f, 1.0f);
		attackparam3.rot = Quaternion::Identity;
		attackparam3.scale = Vector3(2.0f, 2.0f, 2.0f);
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
	//移動速度設定
	Vector3 movespeed = _Player->GetCharaCon().GetMoveSpeed();
	//急に止まらないように
	movespeed.Scale(0.3f);
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();
	//現在のアニメーションナンバー
	int currentanimno = _Player->_Anim->GetPlayAnimNo();
	//アニメーションの再生が終わる && 次のアニメーションがない場合、待機状態に変更
	if (_Player->_Anim->GetPlaying() != true && _Player->_NextAttackAnimNo == Player::AnimationNo::AnimationInvalid)
	{
		_Player->ChangeState(Player::State::Idol);
	}
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
		//コンボ！
 		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}
	//あたり判定作成
	switch (currentanimno)
	{
		//攻撃１の時
	case (int)Player::AnimationNo::AnimationAttack01:
		Attack(_AttackPram[0]);
		break;
		//攻撃２の時
	case (int)Player::AnimationNo::AnimationAttack02:
		Attack(_AttackPram[1]);
		break;
		//攻撃３の時
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
	//攻撃時のあたり判定作成
	//現在のフレーム取得
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == pram.attackframe)
	{
		//攻撃時のサウンド再生。
		_SE->Play(false);
		//攻撃コリジョン作成
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		attack->Create(_Player->_PlayerParam->GiveDamageMass(),pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, _Player->transform);

		_Player->_AttackValue->transform->SetLocalPosition(Vector3(pram.pos.x,pram.pos.y, pram.pos.z));
		string attackvalue = to_string(_Player->_PlayerParam->GiveDamageMass());
		_Player->_AttackValue->SetString(attackvalue.data());
	}
}