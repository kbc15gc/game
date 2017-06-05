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
	//移動速度設定
	Vector3 movespeed = player->GetCharaCon().GetMoveSpeed();
	//急に止まらないように
	movespeed.Scale(0.3f);
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();

	//攻撃時のあたり判定作成
	//現在のフレーム取得
	const int nowFrame = player->_Anim->NowFrame();
	//フレームが10の時あたり判定作成
	if (nowFrame == 10)
	{
		//攻撃コリジョン作成
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		// ※テスト用(後で直してね)。
		{
			Quaternion rot;
			rot = Quaternion::Identity;
			attack->Create(Vector3(0.0f,0.0f,5.0f), rot/*player->transform->GetLocalAngle()*/, Vector3(1.0f,1.0f,5.0f)/*Vector3::one*/, AttackCollision::CollisionMaster::Player, -1.0f, player->transform);
		}
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